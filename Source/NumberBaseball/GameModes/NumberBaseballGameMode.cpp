#include "NumberBaseballGameMode.h"

#include "NumberBaseballGameState.h"
#include "Player/NumberBaseballPlayerState.h"
#include "FunctionLibrary/ComparingNumbersLib.h"
#include "FunctionLibrary/RandomNumberLib.h"
#include "Manager/TurnManager.h"
#include "Player/NumberBaseballPlayerController.h"

ANumberBaseballGameMode::ANumberBaseballGameMode()
	: TurnManager(nullptr), TurnDuration(15.0f), MaxTurnCount(3), MaxGameRound(3), TargetNumberLength(3),
	  RequiredReadyCount(2),
	  ReadyCount(0)
{
	PlayerControllerClass = ANumberBaseballPlayerController::StaticClass();
	GameStateClass = ANumberBaseballGameState::StaticClass();
	PlayerStateClass = ANumberBaseballPlayerState::StaticClass();
}

void ANumberBaseballGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!TurnManager)
	{
		// 턴 매니저 생성
		TurnManager = GetWorld()->SpawnActor<ATurnManager>(TurnManagerClass);
		// 턴 매니저 초기화
		TurnManager->InitTurnManager(TurnDuration, MaxTurnCount);

		// 게임 스테이트에 턴 매니저 저장
		if (ANumberBaseballGameState* NumberBaseballGameState = GetGameState<ANumberBaseballGameState>())
		{
			NumberBaseballGameState->SetTurnManager(TurnManager);
			NumberBaseballGameState->BindTurnManagerEvent();
		}
	}
}

void ANumberBaseballGameMode::PlayerReady(const bool bIsReady)
{
	if (bIsReady)
	{
		ReadyCount++;
	}
	else
	{
		ReadyCount--;
	}

	if (ReadyCount >= RequiredReadyCount)
	{
		StartGame();
	}
}

void ANumberBaseballGameMode::StartGame()
{
	UE_LOG(LogTemp, Warning, TEXT("게임 시작!"));
	// 난수 생성(1 ~ 9 범위)
	TargetNumber = URandomNumberLib::GenerateRandomNumber(TargetNumberLength);

	UE_LOG(LogTemp, Warning, TEXT("TargetNumber: %s"), *TargetNumber);

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ANumberBaseballPlayerController* NumberBaseballPlayerController
			= Cast<ANumberBaseballPlayerController>(It->Get()))
		{
			NumberBaseballPlayerController->Client_PrepareGameStart(TargetNumberLength);
		}
	}

	TurnManager->PrepareTurnStart();
}

void ANumberBaseballGameMode::GotInputText(const int32 PlayerID, const FString& InputText)
{
	// 즉시 턴 종료
	if (TurnManager)
	{
		TurnManager->EndTurn(false);
	}

	int32 StrikeCount = 0;
	int32 BallCount = 0;
	// 숫자 비교
	UComparingNumbersLib::ComparingNumbers(InputText, TargetNumber, StrikeCount, BallCount);

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ANumberBaseballPlayerController* NumberBaseballPlayerController = Cast<
			ANumberBaseballPlayerController>(It->Get()))
		{
			// 채팅 위젯 추가
			NumberBaseballPlayerController->Client_AddChatWidget(PlayerID, InputText);
		}
	}

	FTimerHandle TimerHandle;
	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(
		this,
		&ANumberBaseballGameMode::SendInputText,
		StrikeCount,
		BallCount
	);
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.5f, false);
}

void ANumberBaseballGameMode::TurnTimeOver(const int32 JoinedIndex) const
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ANumberBaseballPlayerController* NumberBaseballPlayerController
			= Cast<ANumberBaseballPlayerController>(It->Get()))
		{
			const FString InputText = TEXT("시간 초과");
			// 채팅 위젯 추가
			NumberBaseballPlayerController->Client_AddChatWidget(JoinedIndex, InputText);
			NumberBaseballPlayerController->Client_UpdateResult(0, 0);
		}
	}
}


void ANumberBaseballGameMode::SendInputText(const int32 StrikeCount, const int32 BallCount)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ANumberBaseballPlayerController* NumberBaseballPlayerController = Cast<
			ANumberBaseballPlayerController>(It->Get()))
		{
			// 플레이어에게 결과 전달
			NumberBaseballPlayerController->Client_UpdateResult(StrikeCount, BallCount);
		}
	}

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda(
		[TurnManager = TurnManager]()
		{
			TurnManager->StartTurn();
		});
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.5f, false);
}
