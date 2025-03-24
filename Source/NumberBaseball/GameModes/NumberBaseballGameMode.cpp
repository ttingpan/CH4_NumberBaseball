#include "NumberBaseballGameMode.h"

#include "NumberBaseballGameState.h"
#include "Player/NumberBaseballPlayerState.h"
#include "FunctionLibrary/ComparingNumbersLib.h"
#include "FunctionLibrary/RandomNumberLib.h"
#include "Manager/TurnManager.h"
#include "Player/NumberBaseballPlayerController.h"

ANumberBaseballGameMode::ANumberBaseballGameMode()
	: TurnManager(nullptr), TurnDuration(5.0f), MaxTurnCount(2), MaxGameRound(3), TargetNumberLength(3),
	  RequiredReadyCount(4),
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

	if (ANumberBaseballGameState* NumberBaseballGameState = GetGameState<ANumberBaseballGameState>())
	{
		// 최대 턴 설정(플레이어 별 턴수 * 총 플레이어 수)
		const int32 CalcMaxTurnCount = MaxTurnCount * NumberBaseballGameState->GetJoinedPlayerCount();
		// 턴 매니저 초기화
		TurnManager->InitTurnManager(TurnDuration, CalcMaxTurnCount);
		
		// 난수 생성(1 ~ 9 범위)
		TargetNumber = URandomNumberLib::GenerateRandomNumber(TargetNumberLength);

		UE_LOG(LogTemp, Warning, TEXT("TargetNumber: %s"), *TargetNumber);

		NumberBaseballGameState->PrepareStartGame(TargetNumberLength);
	}
}

void ANumberBaseballGameMode::GotInputText(ANumberBaseballPlayerState* ChatOwnerPlayerState,
                                           const FString& InputText)
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
		if (ANumberBaseballPlayerController* NumberBaseballPlayerController
			= Cast<ANumberBaseballPlayerController>(It->Get()))
		{
			// 채팅 위젯 추가
			NumberBaseballPlayerController->Client_AddChatWidget(ChatOwnerPlayerState, InputText);
		}
	}

	FTimerHandle TimerHandle;
	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(
		this,
		&ANumberBaseballGameMode::SendInputText,
		StrikeCount,
		BallCount,
		ChatOwnerPlayerState
	);
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.5f, false);
}

void ANumberBaseballGameMode::TurnTimeOver(const ANumberBaseballPlayerState* ChatOwnerPlayerState) const
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ANumberBaseballPlayerController* NumberBaseballPlayerController
			= Cast<ANumberBaseballPlayerController>(It->Get()))
		{
			const FString InputText = TEXT("시간 초과");
			// 채팅 위젯 추가
			NumberBaseballPlayerController->Client_AddChatWidget(ChatOwnerPlayerState, InputText);
			NumberBaseballPlayerController->Client_UpdateResult(0, 0);
		}
	}
}


void ANumberBaseballGameMode::SendInputText(const int32 StrikeCount, const int32 BallCount, ANumberBaseballPlayerState* ChatOwnerPlayerState)
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

	FTimerDelegate TimerDelegate;
	// Hit시 다음 라운드 준비
	if (StrikeCount == 3)
	{
		if (ANumberBaseballGameState* NumberBaseballGameState = GetGameState<ANumberBaseballGameState>())
		{
			NumberBaseballGameState->AddPlayerScore(ChatOwnerPlayerState);
			
			TimerDelegate.BindLambda(
				[NumberBaseballGameState]()
				{
					NumberBaseballGameState->PrepareStartNextRound();
				});
		}
	}
	// 다음 턴 시작
	else
	{
		TimerDelegate.BindLambda(
			[TurnManager = TurnManager]()
			{
				TurnManager->StartTurn();
			});
	}
	
	// 바인드 되어있으면 타이머 실행
	if (TimerDelegate.IsBound())
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.5f, false);
	}
}
