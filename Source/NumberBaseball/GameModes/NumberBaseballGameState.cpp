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
	if (GameStarted)
	{
		UE_LOG(LogTemp, Error, TEXT("게임이 이미 진행 중 입니다."));
		return;
	}

	if (JoinPlayerState && !JoinedPlayerStates.Contains(JoinPlayerState))
	{
		if (JoinedPlayerStates.Num() == 4)
		{
			UE_LOG(LogTemp, Error, TEXT("더 이상 참가할 수 없습니다."));
			return;
		}

		JoinPlayerState->SetPlayerName(NewPlayerName);

		UE_LOG(LogTemp, Warning, TEXT("Join Player : %s"), *NewPlayerName);

		const int32 JoinedIndex = JoinedPlayerStates.Num();

		JoinedPlayerStates.Add(JoinPlayerState);
		JoinedPlayerControllers.Add(JoinPlayerState->GetNumberBaseballPlayerController());

		JoinPlayerState->GetNumberBaseballPlayerController()->SetJoinedIndex(JoinedIndex);

		if (ANumberBaseballGameMode* NumberBaseballGameMode
			= Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode()))
		{
			if (ANumberBaseballPlayerController* NumberBaseballPlayerController
				= JoinPlayerState->GetNumberBaseballPlayerController())
			{
				NumberBaseballPlayerController->Client_JoinGame(JoinedIndex, NumberBaseballGameMode->GetWinScore());
			}

			NumberBaseballGameMode->SetRequiredReadyCount(JoinedPlayerStates.Num());
		}

		// 준비 버튼 표시 업데이트
		UpdateVisibilityReadyTextBorder();

		UpdateOtherPlayer();
	}
}

void ANumberBaseballGameState::UpdateOtherPlayer()
{
	for (const ANumberBaseballPlayerController* NumberBaseballPlayerController : JoinedPlayerControllers)
	{
		if (NumberBaseballPlayerController)
		{
			for (int32 i = 0; i < JoinedPlayerStates.Num(); i++)
			{
				if (const ANumberBaseballPlayerState* NumberBaseballPlayerState = JoinedPlayerStates[i])
				{
					NumberBaseballPlayerController->Client_UpdateOtherPlayerName(
						i, NumberBaseballPlayerState->GetPlayerName());
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
	for (int32 i = 0; i < JoinedPlayerStates.Num(); i++)
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

	// 현재 최대 턴일 경우
	if (TurnManager->IsMaxTurn())
	{
		PrepareStartNextRound();
	}

	if (ANumberBaseballPlayerController* NumberBaseballPlayerController
		= JoinedPlayerStates[CurrentTurnPlayerIndex]->GetNumberBaseballPlayerController())
	{
		NumberBaseballPlayerController->Client_OnTurnEnded(false);
	}

	// 타이머 텍스트 초기화
	for (ANumberBaseballPlayerController* NumberBaseballPlayerController : JoinedPlayerControllers)
	{
		if (NumberBaseballPlayerController)
		{
			NumberBaseballPlayerController->Client_OnTurnUpdated(TurnManager->GetTurnDuration());
		}
	}
}

void ANumberBaseballGameState::PrepareStartGame(const int32 TargetNumberLength)
{
	GameStarted = true;

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
	BeforeTurnPlayerController = nullptr;

	// 3초 후 다음 라운드 시작
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		this,
		&ANumberBaseballGameState::StartNextRound,
		1.0f,
		false
	);
}

void ANumberBaseballGameState::StartNextRound()
{
	// 라운드 갱신
	CurrentRound++;

	CurrentTurnPlayerIndex = 0;
	TurnManager->SetCurrentTurnCount(0);
	UpdateRoundText();

	Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode())->StartGame();
}

void ANumberBaseballGameState::AddChatWidget(const int32 Index, const FString& InputText)
{
	for (int32 i = 0; i < JoinedPlayerControllers.Num(); i++)
	{
		if (JoinedPlayerControllers[i])
		{
			if (JoinedPlayerStates.Num() > Index && JoinedPlayerStates[Index])
			{
				JoinedPlayerControllers[i]->Client_AddChatWidget(JoinedPlayerStates[Index], InputText);
			}
		}
	}
}

void ANumberBaseballGameState::UpdateVisibilityReadyTextBorder() const
{
	for (int32 i = 0; i < JoinedPlayerStates.Num(); i++)
	{
		if (JoinedPlayerStates[i] && JoinedPlayerControllers[i])
		{
			for (ANumberBaseballPlayerController* NumberBaseballPlayerController : JoinedPlayerControllers)
			{
				if (NumberBaseballPlayerController && NumberBaseballPlayerController != JoinedPlayerControllers[i])
				{
					NumberBaseballPlayerController->Client_SetVisibilityReadyTextBorder(
						i, JoinedPlayerStates[i]->IsReady());
				}
			}
		}
	}
}

void ANumberBaseballGameState::UpdateVisibilityReadyTextBorder(const int32 Index, const bool bIsReady) const
{
	if (JoinedPlayerControllers.Num() > Index && JoinedPlayerControllers[Index])
	{
		for (ANumberBaseballPlayerController* NumberBaseballPlayerController : JoinedPlayerControllers)
		{
			if (NumberBaseballPlayerController)
			{
				NumberBaseballPlayerController->Client_SetVisibilityReadyTextBorder(Index, bIsReady);
			}
		}
	}
}

void ANumberBaseballGameState::AddPlayerScore(const int32 Index, const int32 WinScore)
{
	if (JoinedPlayerStates.Num() > Index)
	{
		if (ANumberBaseballPlayerState* NumberBaseballPlayerState = JoinedPlayerStates[Index])
		{
			const int32 NewScore = NumberBaseballPlayerState->GetScore() + 1;
			NumberBaseballPlayerState->SetScore(NewScore);

			for (int32 i = 0; i < JoinedPlayerControllers.Num(); i++)
			{
				if (JoinedPlayerControllers[i])
				{
					JoinedPlayerControllers[i]->Client_UpdateScore(Index, NewScore);
				}
			}

			FTimerDelegate TimerDelegate;

			float InRate = 0.5f;
			// 승리 가능 점수에 도달 했다면
			if (NewScore == WinScore)
			{
				InRate = 1.0f;
				TimerDelegate.BindUFunction(this, FName("ShowGameOverWidget"), NumberBaseballPlayerState);
			}
			else
			{
				TimerDelegate.BindUFunction(this, FName("PrepareStartNextRound"));
			}

			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(
				TimerHandle,
				TimerDelegate,
				InRate,
				false
			);
		}
	}
}

void ANumberBaseballGameState::UpdateHostGameStartButtonIsEnabled()
{
	// 혼자서는 게임시작 불가
	if (JoinedPlayerStates.Num() == 1)
	{
		JoinedPlayerControllers[0]->Client_SetReadyButtonIsEnabled(false);
		return;
	}

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

void ANumberBaseballGameState::ShowGameOverWidget(const APlayerState* WinnerPlayerState)
{
	for (ANumberBaseballPlayerController* NumberBaseballPlayerController : JoinedPlayerControllers)
	{
		if (NumberBaseballPlayerController)
		{
			NumberBaseballPlayerController->Client_ShowGameOverWidget(
				WinnerPlayerState == NumberBaseballPlayerController->PlayerState, WinnerPlayerState);
		}
	}

	CurrentRound = 1;
	CurrentTurnPlayerIndex = 0;
	TurnManager->SetCurrentTurnCount(0);
	JoinedPlayerStates.Empty();
	JoinedPlayerControllers.Empty();
	BeforeTurnPlayerController = nullptr;
	GameStarted = false;
}
