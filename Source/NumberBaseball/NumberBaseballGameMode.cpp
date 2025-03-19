#include "NumberBaseballGameMode.h"

#include "NumberBaseballGameState.h"
#include "NumberBaseballPlayerState.h"
#include "FunctionLibrary/ComparingNumbersLib.h"
#include "FunctionLibrary/RandomNumberLib.h"
#include "Manager/TurnManager.h"
#include "Player/NumberBaseballPlayerController.h"

ANumberBaseballGameMode::ANumberBaseballGameMode()
	: TurnManager(nullptr), TurnDuration(15.0f), MaxTurnCount(3), MaxGameRound(3), TargetNumberLength(3),
	  RequiredReadyCount(2),
	  JoinedPlayerCount(0), ReadyCount(0)
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

void ANumberBaseballGameMode::JoinGame(ANumberBaseballPlayerController* PlayerController,
                                       const FString& PlayerName)
{
	if (PlayerController && PlayerController->PlayerState)
	{
		if (JoinedPlayerCount == 2)
		{
			UE_LOG(LogTemp, Error, TEXT("더 이상 참가할 수 없습니다."));
			return;
		}

		JoinedPlayerCount++;
		PlayerController->PlayerState->SetPlayerName(PlayerName);
		if (ANumberBaseballGameState* NumberBaseballGameState = GetGameState<ANumberBaseballGameState>())
		{
			// 플레이어 등록
			NumberBaseballGameState->RegisterPlayer(PlayerController, PlayerName);
		}
	}
}

void ANumberBaseballGameMode::PlayerReady(const FString& PlayerName, const bool bIsReady)
{
	FString LogText = FString::Printf(TEXT("플레이어 %s "), *PlayerName);
	if (bIsReady)
	{
		ReadyCount++;
		LogText += TEXT("준비 완료");
	}
	else
	{
		ReadyCount--;
		LogText += TEXT("준비 취소");
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *LogText);

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
		if (ANumberBaseballPlayerController* NumberBaseballPlayerController = Cast<
			ANumberBaseballPlayerController>(It->Get()))
		{
			NumberBaseballPlayerController->Client_PrepareGameStart(TargetNumberLength);
		}
	}

	TurnManager->PrepareTurnStart();
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

void ANumberBaseballGameMode::Server_GotInputText_Implementation(const FString& PlayerName, const FString& InputText)
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
	// 결과
	const FString Result =
		FString::Printf(TEXT("%s가 입력한 %s의 결과: %dS %dB"), *PlayerName, *InputText, StrikeCount, BallCount);

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (const ANumberBaseballPlayerController* NumberBaseballPlayerController = Cast<
			ANumberBaseballPlayerController>(It->Get()))
		{
			// 채팅 위젯 추가
			NumberBaseballPlayerController->Client_AddChatWidget(PlayerName, InputText);
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

bool ANumberBaseballGameMode::Server_GotInputText_Validate(const FString& PlayerName, const FString& InputText)
{
	// 필요시 유효성 체크 로직 구현
	return true;
}
