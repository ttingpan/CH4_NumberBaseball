PRAGMA_DISABLE_OPTIMIZATION

#include "NumberBaseballGameState.h"

#include "NumberBaseballGameMode.h"
#include "GameFramework/PlayerState.h"
#include "Manager/TurnManager.h"
#include "Net/UnrealNetwork.h"
#include "Player/NumberBaseballPlayerController.h"

void ANumberBaseballGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANumberBaseballGameState, PlayerNames);
	DOREPLIFETIME(ANumberBaseballGameState, CurrentTurnPlayerIndex);
	DOREPLIFETIME(ANumberBaseballGameState, TurnManager);
	DOREPLIFETIME(ANumberBaseballGameState, CurrentRound);
}

void ANumberBaseballGameState::RegisterPlayer(ANumberBaseballPlayerController* PlayerController
                                              , const FString& PlayerName)
{
	PlayerNames.Add(PlayerName);
	PlayerControllers.Add(PlayerController);
	UE_LOG(LogTemp, Warning, TEXT("RegisterPlayer: %s"), *PlayerName);

	// 준비 버튼 초기화
	PlayerController->Client_InitReadyButton(PlayerNames.Num() == 1);

	// 상대 이름 설정
	Multicast_UpdateOtherPlayerName();
}

void ANumberBaseballGameState::BindTurnManagerEvent()
{
	if (TurnManager)
	{
		TurnManager->OnTurnUpdated.AddDynamic(this, &ANumberBaseballGameState::OnTurnUpdated);
		TurnManager->OnTurnEnded.AddDynamic(this, &ANumberBaseballGameState::OnTurnEnded);
		TurnManager->OnTurnStarted.AddDynamic(this, &ANumberBaseballGameState::OnTurnStarted);
		TurnManager->OnTurnEndedImmediately.AddDynamic(this, &ANumberBaseballGameState::OnTurnEndedImmediately);
	}
}

void ANumberBaseballGameState::OnTurnStarted()
{
	for (ANumberBaseballPlayerController* PlayerController : PlayerControllers)
	{
		if (PlayerController)
		{
			// 현재 플레이어 턴 시작
			if (PlayerController == PlayerControllers[CurrentTurnPlayerIndex])
			{
				PlayerController->Client_OnTurnStarted();
			}
			// 이전 플레이어 턴 종료
			else
			{
				PlayerController->Client_OnTurnEnded(true);
			}
		}
	}
}

void ANumberBaseballGameState::OnTurnUpdated(const float RemainingTime)
{
	for (ANumberBaseballPlayerController* PlayerController : PlayerControllers)
	{
		if (PlayerController)
		{
			PlayerController->Client_OnTurnUpdated(RemainingTime);
		}
	}
}

void ANumberBaseballGameState::OnTurnEnded()
{
	// 게임 시작 준비 턴일 경우
	if (TurnManager->GetCurrentTurnCount() == 0)
	{
		// 게임 시작
		TurnManager->StartTurn();
		OnTurnStarted();
		return;
	}

	// 현재 턴 플레이어 턴 종료
	PlayerControllers[CurrentTurnPlayerIndex]->Client_OnTurnEnded(true);

	// 다음 턴 플레이어 설정
	CurrentTurnPlayerIndex++;
	CurrentTurnPlayerIndex %= 2;

	// 현재 최대 턴일 경우
	if (TurnManager->IsMaxTurn())
	{
		// 라운드 종료
		CurrentRound++;
		// 현재 최대 라운드일 경우
		if (CurrentRound == Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode())->GetMaxGameRound())
		{
			// TODO: 최종 게임 종료후 승/패 확인
		}
		else
		{
			// TODO: 현재 라운드 승/패 확인 후 적용

			// 3초 후 다음 라운드 시작
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(
				TimerHandle,
				this,
				&ANumberBaseballGameState::StartNextRound,
				3.0f,
				false
			);
		}
	}
	// 최대 턴이 아닐 경우
	else
	{
		// 다음 턴 플레이어 턴 시작
		PlayerControllers[CurrentTurnPlayerIndex]->Client_OnTurnStarted();

		// 턴 시작
		TurnManager->StartTurn();
	}
}

void ANumberBaseballGameState::OnTurnEndedImmediately()
{
	// 현재 턴 플레이어 턴 종료
	PlayerControllers[CurrentTurnPlayerIndex]->Client_OnTurnEnded(false);

	// 다음 턴 플레이어 설정
	CurrentTurnPlayerIndex++;
	CurrentTurnPlayerIndex %= 2;

	PlayerControllers[CurrentTurnPlayerIndex]->Client_OnTurnEnded(false);

	// 타이머 텍스트 초기화
	for (ANumberBaseballPlayerController* PlayerController : PlayerControllers)
	{
		if (PlayerController)
		{
			PlayerController->Client_OnTurnUpdated(TurnManager->GetTurnDuration());
		}
	}
}

void ANumberBaseballGameState::StartNextRound() const
{
	TurnManager->PrepareTurnStart();
}

void ANumberBaseballGameState::Multicast_UpdateOtherPlayerName_Implementation()
{
	if (PlayerNames.Num() <= 1)
	{
		return;
	}

	for (const APlayerState* PlayerState : PlayerArray)
	{
		if (const ANumberBaseballPlayerController* NumberBaseballPlayerController
			= Cast<ANumberBaseballPlayerController>(PlayerState->GetPlayerController()))
		{
			if (PlayerState->GetPlayerName() == PlayerNames[0])
			{
				NumberBaseballPlayerController->Client_SetOtherPlayerName(PlayerNames[1]);
			}
			else
			{
				NumberBaseballPlayerController->Client_SetOtherPlayerName(PlayerNames[0]);
			}
		}
	}
}
