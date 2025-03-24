PRAGMA_DISABLE_OPTIMIZATION

#include "NumberBaseballGameState.h"

#include "NumberBaseballGameMode.h"
#include "Manager/TurnManager.h"
#include "Net/UnrealNetwork.h"
#include "Player/NumberBaseballPlayerState.h"
#include "Player/NumberBaseballPlayerController.h"

void ANumberBaseballGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANumberBaseballGameState, JoinedPlayerStates);
	DOREPLIFETIME(ANumberBaseballGameState, JoinedPlayerControllers);
	DOREPLIFETIME(ANumberBaseballGameState, CurrentTurnPlayerIndex);
	DOREPLIFETIME(ANumberBaseballGameState, TurnManager);
	DOREPLIFETIME(ANumberBaseballGameState, CurrentRound);
}

void ANumberBaseballGameState::JoinGame(ANumberBaseballPlayerState* JoinPlayerState, const FString& NewPlayerName)
{
	if (JoinPlayerState && !JoinedPlayerStates.Contains(JoinPlayerState))
	{
		if (JoinedPlayerStates.Num() == 4)
		{
			UE_LOG(LogTemp, Error, TEXT("더 이상 참가할 수 없습니다."));
			return;
		}

		JoinPlayerState->SetPlayerName(NewPlayerName);

		UE_LOG(LogTemp, Warning, TEXT("NewPlayer Name : %s"), *NewPlayerName);

		JoinedPlayerStates.Add(JoinPlayerState);
		JoinedPlayerControllers.Add(JoinPlayerState->GetNumberBaseballPlayerController());

		if (ANumberBaseballPlayerController* NumberBaseballPlayerController
			= JoinPlayerState->GetNumberBaseballPlayerController())
		{
			NumberBaseballPlayerController->Client_JoinGame(JoinedPlayerStates.Num() == 1);
		}

		if (ANumberBaseballGameMode* NumberBaseballGameMode
			= Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode()))
		{
			NumberBaseballGameMode->SetRequiredReadyCount(JoinedPlayerStates.Num());
		}

		UpdateOtherPlayer();
	}
}

void ANumberBaseballGameState::UpdateOtherPlayer()
{
	for (const ANumberBaseballPlayerController* NumberBaseballPlayerController : JoinedPlayerControllers)
	{
		if (NumberBaseballPlayerController)
		{
			int32 Index = 1;
			for (const ANumberBaseballPlayerState* NumberBaseballPlayerState : JoinedPlayerStates)
			{
				if (NumberBaseballPlayerState)
				{
					int32 TargetIndex = 0;
					if (NumberBaseballPlayerState != NumberBaseballPlayerController->PlayerState)
					{
						TargetIndex = Index++;
					}

					NumberBaseballPlayerController->Client_UpdateOtherPlayerName(
						TargetIndex,
						NumberBaseballPlayerState->GetPlayerName());
				}
			}
		}
	}
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
	// 이전 플레이어 종료
	if (BeforeTurnPlayerController)
	{
		BeforeTurnPlayerController->Client_OnTurnEnded(true);
	}

	// 현재 플레이어 시작
	if (JoinedPlayerControllers[CurrentTurnPlayerIndex])
	{
		UpdateTurnText();
		JoinedPlayerControllers[CurrentTurnPlayerIndex]->Client_OnTurnStarted();
		BeforeTurnPlayerController = JoinedPlayerControllers[CurrentTurnPlayerIndex];
	}

	for (const ANumberBaseballPlayerController* NumberBaseballPlayerController : JoinedPlayerControllers)
	{
		NumberBaseballPlayerController->Client_UpdatePlayerSlotWidget();
	}
}

void ANumberBaseballGameState::OnTurnUpdated(const float RemainingTime)
{
	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		if (ANumberBaseballPlayerController* NumberBaseballPlayerController
			= JoinedPlayerStates[i]->GetNumberBaseballPlayerController())
		{
			NumberBaseballPlayerController->Client_OnTurnUpdated(RemainingTime);
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

	Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode())->TurnTimeOver(
		JoinedPlayerStates[CurrentTurnPlayerIndex]);

	// 현재 턴 플레이어 턴 종료
	if (ANumberBaseballPlayerController* NumberBaseballPlayerController
		= JoinedPlayerStates[CurrentTurnPlayerIndex]->GetNumberBaseballPlayerController())
	{
		NumberBaseballPlayerController->Client_OnTurnEnded(true);
	}

	// 다음 턴 플레이어 설정
	CurrentTurnPlayerIndex++;
	CurrentTurnPlayerIndex %= JoinedPlayerStates.Num();

	// 현재 최대 턴일 경우
	if (TurnManager->IsMaxTurn())
	{
		PrepareStartNextRound();
	}
	// 최대 턴이 아닐 경우
	else
	{
		ANumberBaseballPlayerController* PlayerController
			= JoinedPlayerStates[CurrentTurnPlayerIndex]->GetNumberBaseballPlayerController();

		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda(
			[TurnManager = TurnManager, PlayerController = PlayerController]()
			{
				// 다음 턴 플레이어 턴 시작
				PlayerController->Client_OnTurnStarted();

				TurnManager->StartTurn();
			});

		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.0f, false);
	}
}

void ANumberBaseballGameState::OnTurnEndedImmediately()
{
	// 현재 턴 플레이어 턴 종료
	if (ANumberBaseballPlayerController* NumberBaseballPlayerController
		= JoinedPlayerStates[CurrentTurnPlayerIndex]->GetNumberBaseballPlayerController())
	{
		NumberBaseballPlayerController->Client_OnTurnEnded(false);
	}

	// 다음 턴 플레이어 설정
	CurrentTurnPlayerIndex++;
	CurrentTurnPlayerIndex %= JoinedPlayerStates.Num();

	if (ANumberBaseballPlayerController* NumberBaseballPlayerController
		= JoinedPlayerStates[CurrentTurnPlayerIndex]->GetNumberBaseballPlayerController())
	{
		NumberBaseballPlayerController->Client_OnTurnEnded(false);
	}

	// 타이머 텍스트 초기화
	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		if (ANumberBaseballPlayerController* NumberBaseballPlayerController = JoinedPlayerControllers[i])
		{
			NumberBaseballPlayerController->Client_OnTurnUpdated(TurnManager->GetTurnDuration());
		}
	}
}

void ANumberBaseballGameState::PrepareStartGame(const int32 TargetNumberLength)
{
	for (ANumberBaseballPlayerController* NumberBaseballPlayerController : JoinedPlayerControllers)
	{
		if (NumberBaseballPlayerController)
		{
			NumberBaseballPlayerController->Client_PrepareGameStart(TargetNumberLength);
			NumberBaseballPlayerController->Client_AddChatRoundNotifyWidget(CurrentRound, true);
		}
	}

	TurnManager->PrepareTurnStart();
}

void ANumberBaseballGameState::PrepareStartNextRound()
{
	// 현재 최대 라운드일 경우
	if (const ANumberBaseballGameMode* NumberBaseballGameMode = Cast<ANumberBaseballGameMode>(
		GetWorld()->GetAuthGameMode()))
	{
		if (CurrentRound == NumberBaseballGameMode->GetMaxGameRound())
		{
			// TODO 최종 게임 종료후 승/패 확인
			UE_LOG(LogTemp, Warning, TEXT("GameOver!"));
			return;
		}
	}

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

void ANumberBaseballGameState::StartNextRound()
{
	// for (ANumberBaseballPlayerController* NumberBaseballPlayerController : JoinedPlayerControllers)
	// {
	// 	if (NumberBaseballPlayerController)
	// 	{
	// 		NumberBaseballPlayerController->Client_AddChatRoundNotifyWidget(CurrentRound, false);
	// 	}
	// }

	// 라운드 갱신
	CurrentRound++;

	CurrentTurnPlayerIndex = 0;
	TurnManager->SetCurrentTurnCount(0);
	UpdateRoundText();

	Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode())->StartGame();
}

void ANumberBaseballGameState::AddPlayerScore(ANumberBaseballPlayerState* WinnerPlayerState)
{
	const int32 NewScore = WinnerPlayerState->GetScore() + 1;
	WinnerPlayerState->SetScore(NewScore);

	for (ANumberBaseballPlayerController* NumberBaseballPlayerController : JoinedPlayerControllers)
	{
		if (NumberBaseballPlayerController
			&& NumberBaseballPlayerController == WinnerPlayerState->GetPlayerController())
		{
			NumberBaseballPlayerController->Client_UpdateScore(WinnerPlayerState, NewScore);
		}
	}
}

void ANumberBaseballGameState::UpdateHostGameStartButtonIsEnabled()
{
	for (int32 i = 1; i < JoinedPlayerStates.Num(); i++)
	{
		// 참가한 인원 중 한명이라도 준비 안했으면 게임시작 불가
		if (!JoinedPlayerStates[i]->IsReady())
		{
			JoinedPlayerControllers[0]->Client_SetReadyButtonIsEnabled(false);
			return;
		}
	}

	// 모두 준비 했다면 게임시작 버튼 활성화
	JoinedPlayerControllers[0]->Client_SetReadyButtonIsEnabled(true);
}

void ANumberBaseballGameState::UpdateRoundText()
{
	for (ANumberBaseballPlayerController* NumberBaseballPlayerController : JoinedPlayerControllers)
	{
		NumberBaseballPlayerController->Client_UpdateRoundText(CurrentRound);
	}

	UpdateTurnText();
}

void ANumberBaseballGameState::UpdateTurnText()
{
	for (ANumberBaseballPlayerController* NumberBaseballPlayerController : JoinedPlayerControllers)
	{
		NumberBaseballPlayerController->Client_UpdateTurnText(TurnManager->GetCurrentTurnCount());
	}
}
