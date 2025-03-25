#include "NumberBaseballPlayerController.h"

#include "NumberBaseballPlayerState.h"
#include "GameFramework/GameUserSettings.h"
#include "GameModes/NumberBaseballGameState.h"
#include "Net/UnrealNetwork.h"
#include "NumberBaseball/GameModes/NumberBaseballGameMode.h"
#include "UI/MainWidget.h"
#include "UI/NumberBaseballHUD.h"
#include "UI/JoinGameWidget.h"

void ANumberBaseballPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// UI 모드 설정
	SetInputMode(FInputModeUIOnly());
	// 마우스 커서 보이기
	bShowMouseCursor = true;

	// 화면 크기 설정
	ChangeGameResolution();
}

void ANumberBaseballPlayerController::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANumberBaseballPlayerController, JoinGameWidget);
	DOREPLIFETIME(ANumberBaseballPlayerController, MainWidget);
	DOREPLIFETIME(ANumberBaseballPlayerController, JoinedIndex);
}

void ANumberBaseballPlayerController::ChangeGameResolution()
{
	// 게임 사용자 설정 가져오기
	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		// 해상도 및 전체화면 모드 설정
		Settings->SetScreenResolution({1280, 720});
		Settings->SetFullscreenMode(EWindowMode::Windowed);

		// 변경 사항 적용
		Settings->ApplySettings(false);
	}
}

void ANumberBaseballPlayerController::Client_SetVisibilityReadyTextBorder_Implementation(const int32 Index,
	const bool bIsVisible)
{
	if (MainWidget)
	{
		MainWidget->SetVisibilityReadyTextBorder(Index, bIsVisible);
	}
}

void ANumberBaseballPlayerController::Client_UpdatePlayerSlotWidget_Implementation() const
{
	if (MainWidget)
	{
		if (const ANumberBaseballGameState* NumberBaseballGameState
			= Cast<ANumberBaseballGameState>(GetWorld()->GetGameState()))
		{
			for (int32 i = 0; i < NumberBaseballGameState->GetJoinedPlayerStates().Num(); i++)
			{
				if (NumberBaseballGameState->GetJoinedPlayerStates()[i])
				{
					const bool bIsMyTurn = NumberBaseballGameState->GetCurrentTurnPlayerIndex() == i;
					MainWidget->UpdatePlayerSlotWidgetByIndex(i, bIsMyTurn);
				}
			}
		}
	}
}

void ANumberBaseballPlayerController::Client_UpdateOtherPlayerName_Implementation(
	const int32 Index, const FString& OtherPlayerName) const
{
	if (MainWidget)
	{
		MainWidget->SetPlayerName(Index, OtherPlayerName);
	}
}

void ANumberBaseballPlayerController::Server_JoinGame_Implementation(const FString& NewPlayerName)
{
	if (ANumberBaseballGameState* NumberBaseballGameState = Cast<ANumberBaseballGameState>(GetWorld()->GetGameState()))
	{
		if (ANumberBaseballPlayerState* NumberBaseballPlayerState = Cast<ANumberBaseballPlayerState>(PlayerState))
		{
			NumberBaseballGameState->JoinGame(NumberBaseballPlayerState, NewPlayerName);
		}
	}
}

bool ANumberBaseballPlayerController::Server_JoinGame_Validate(const FString& NewPlayerName)
{
	return true;
}

void ANumberBaseballPlayerController::Client_JoinGame_Implementation(const int32 Index, const int32 WinScore)
{
	if (GetHUD())
	{
		if (const ANumberBaseballHUD* NumberBaseballHUD = Cast<ANumberBaseballHUD>(GetHUD()))
		{
			NumberBaseballHUD->JoinGame(Index, WinScore);

			if (MainWidget)
			{
				MainWidget->InitReadyButton(Index == 0);
			}
		}
	}
}

void ANumberBaseballPlayerController::Client_OnTurnStarted_Implementation()
{
	if (MainWidget)
	{
		MainWidget->TurnStarted();
	}
}

void ANumberBaseballPlayerController::Client_OnTurnEnded_Implementation(const bool bIsAuto)
{
	if (MainWidget)
	{
		MainWidget->TurnEnded(bIsAuto);
	}
}

void ANumberBaseballPlayerController::Client_OnTurnUpdated_Implementation(const float RemainingTime)
{
	if (MainWidget)
	{
		MainWidget->UpdateTimerText(FString::FromInt(RemainingTime));
	}
}

void ANumberBaseballPlayerController::Client_SetReadyButtonText_Implementation(const bool bIsReady)
{
	if (MainWidget)
	{
		MainWidget->SetReadyButtonText(bIsReady);
	}
}

void ANumberBaseballPlayerController::Client_SetReadyButtonIsEnabled_Implementation(const bool bIsReady)
{
	if (MainWidget)
	{
		MainWidget->SetReadyButtonIsEnabled(bIsReady);
	}
}

void ANumberBaseballPlayerController::Client_UpdateResult_Implementation(const int32 StrikeCount, const int32 BallCount)
{
	if (MainWidget)
	{
		MainWidget->UpdateResult(StrikeCount, BallCount);
	}
}

void ANumberBaseballPlayerController::Client_AddChatWidget_Implementation(
	const ANumberBaseballPlayerState* ChatOwnerPlayerState, const FString& InputText)
{
	if (GetHUD())
	{
		if (const ANumberBaseballHUD* NumberBaseballHUD = Cast<ANumberBaseballHUD>(GetHUD()))
		{
			NumberBaseballHUD->AddChatWidget(ChatOwnerPlayerState == PlayerState, ChatOwnerPlayerState->GetPlayerName(), InputText);
		}
	}
}

void ANumberBaseballPlayerController::Client_UpdateRoundText_Implementation(const int32 CurrentRound)
{
	if (MainWidget)
	{
		MainWidget->UpdateRoundText(CurrentRound);
	}
}

void ANumberBaseballPlayerController::Client_UpdateTurnText_Implementation(const int32 CurrentTurn)
{
	if (MainWidget)
	{
		MainWidget->UpdateTurnText(CurrentTurn);
	}
}

void ANumberBaseballPlayerController::Client_AddChatRoundNotifyWidget_Implementation(const int32 CurrentRound,
	const bool bIsStart)
{
	if (GetHUD())
	{
		if (const ANumberBaseballHUD* NumberBaseballHUD = Cast<ANumberBaseballHUD>(GetHUD()))
		{
			NumberBaseballHUD->AddChatRoundNotifyWidget(CurrentRound, bIsStart);
		}
	}
}

void ANumberBaseballPlayerController::Client_UpdateScore_Implementation(const int32 WinnerPlayerIndex,
                                                                        const int32 Score)
{
	if (MainWidget)
	{
		MainWidget->UpdateScore(WinnerPlayerIndex, Score);
	}
}

void ANumberBaseballPlayerController::SetMainWidget(UMainWidget* InMainWidget)
{
	if (InMainWidget)
	{
		MainWidget = InMainWidget;
		MainWidget->OnInputCommittedDelegate.AddDynamic(this, &ANumberBaseballPlayerController::Server_SetInputText);
		MainWidget->OnReadyButtonClickedDelegate.AddDynamic(
			this, &ANumberBaseballPlayerController::Server_ReadyButtonClicked);
	}
}

void ANumberBaseballPlayerController::SetJoinButtonIsEnabled() const
{
	if (JoinGameWidget)
	{
		JoinGameWidget->SetJoinButtonIsEnabled();
	}
}

void ANumberBaseballPlayerController::Client_ShowGameOverWidget_Implementation(const bool bIsWin, const APlayerState* WinnerPlayerState)
{
	if (GetHUD())
	{
		if (const ANumberBaseballHUD* NumberBaseballHUD = Cast<ANumberBaseballHUD>(GetHUD()))
		{
			NumberBaseballHUD->ShowGameOverWidget(bIsWin, WinnerPlayerState);
		}
	}
}

void ANumberBaseballPlayerController::Client_PrepareGameStart_Implementation(const int32& TargetNumberLength)
{
	if (MainWidget)
	{
		MainWidget->PrepareStartTurn(TargetNumberLength);
	}
}

void ANumberBaseballPlayerController::Server_ReadyButtonClicked_Implementation()
{
	if (ANumberBaseballPlayerState* NumberBaseballPlayerState = GetPlayerState<ANumberBaseballPlayerState>())
	{
		NumberBaseballPlayerState->GameReady(JoinedIndex);
	}
}

bool ANumberBaseballPlayerController::Server_ReadyButtonClicked_Validate()
{
	return true;
}

void ANumberBaseballPlayerController::Server_SetInputText_Implementation(const FString& InputText)
{
	if (ANumberBaseballGameMode* GameMode = Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->GotInputText(JoinedIndex, InputText);
	}
}

bool ANumberBaseballPlayerController::Server_SetInputText_Validate(const FString& InputText)
{
	// 필요시 유효성 체크 로직 구현
	return true;
}
