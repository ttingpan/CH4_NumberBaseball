PRAGMA_DISABLE_OPTIMIZATION

#include "NumberBaseballPlayerController.h"

#include "NumberBaseballPlayerState.h"
#include "GameFramework/GameUserSettings.h"
#include "GameModes/NumberBaseballGameState.h"
#include "NumberBaseball/GameModes/NumberBaseballGameMode.h"
#include "UI/MainWidget.h"
#include "UI/NumberBaseballHUD.h"

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

void ANumberBaseballPlayerController::ChangeGameResolution()
{
	// 게임 사용자 설정 가져오기
	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		// 해상도 및 전체화면 모드 설정
		Settings->SetScreenResolution({1280, 720});
		Settings->SetFullscreenMode(EWindowMode::Windowed);

		// 변경 사항 적용 (false: 즉시 적용하지 않으면 false, true: 재시작 전까지 적용)
		Settings->ApplySettings(false);
	}
}

void ANumberBaseballPlayerController::SetPlayerName(const FString& NewPlayerName) const
{
	// 클라이언트 player state에 이름 설정
	PlayerState->SetPlayerName(NewPlayerName);
	if (ANumberBaseballGameState* NumberBaseballGameState = Cast<ANumberBaseballGameState>(GetWorld()->GetGameState()))
	{
		NumberBaseballGameState->Server_JoinGame();
	}
}

void ANumberBaseballPlayerController::Server_SetPlayerName_Implementation(const FString& NewPlayerName)
{
	if (ANumberBaseballPlayerState* NumberBaseballPlayerState = Cast<ANumberBaseballPlayerState>(PlayerState))
	{
		// 서버 player state에 이름 설정
		NumberBaseballPlayerState->SetPlayerName(NewPlayerName);
	}
}

bool ANumberBaseballPlayerController::Server_SetPlayerName_Validate(const FString& NewPlayerName)
{
	return true;
}

void ANumberBaseballPlayerController::Client_UpdatePlayerSlotWidget_Implementation()
{
	if (MainWidget)
	{
		if (ANumberBaseballGameState* NumberBaseballGameState
			= Cast<ANumberBaseballGameState>(GetWorld()->GetGameState()))
		{
			TArray<FUniqueNetIdRepl> JoinedPlayerIDs;
			NumberBaseballGameState->GetJoinedPlayerIDs(JoinedPlayerIDs);
			for (int i = 0; i < JoinedPlayerIDs.Num(); i++)
			{
				if (JoinedPlayerIDs[i] != PlayerState->GetUniqueId())
				{
					if (const ANumberBaseballPlayerController* NumberBaseballPlayerController
						= NumberBaseballGameState->GetPlayerControllerByUniqueID(JoinedPlayerIDs[i]))
					{
						MainWidget->SetPlayerName(i, NumberBaseballPlayerController->PlayerState->GetPlayerName());
					}
				}
			}
		}
	}
}

void ANumberBaseballPlayerController::Client_JoinGame_Implementation(const int32 PlayerIndex)
{
	if (GetHUD())
	{
		if (const ANumberBaseballHUD* NumberBaseballHUD = Cast<ANumberBaseballHUD>(GetHUD()))
		{
			NumberBaseballHUD->JoinGame();

			if (MainWidget)
			{
				MainWidget->InitReadyButton(PlayerIndex == 0);
				Client_UpdatePlayerSlotWidget_Implementation();
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

void ANumberBaseballPlayerController::Client_AddChatWidget_Implementation(const int32 JoinedIndex,
                                                                          const FString& InputText)
{
	if (GetHUD())
	{
		if (const ANumberBaseballHUD* NumberBaseballHUD = Cast<ANumberBaseballHUD>(GetHUD()))
		{
			// const FString PlayerName
			// 	= Cast<ANumberBaseballGameState>(GetWorld()->GetGameState())->GetPlayerNameByID(PlayerID);
			// NumberBaseballHUD->AddChatWidget(PlayerName, InputText);
		}
	}
}

void ANumberBaseballPlayerController::Client_UpdateResult_Implementation(const int32 StrikeCount, const int32 BallCount)
{
	if (MainWidget)
	{
		MainWidget->UpdateResult(StrikeCount, BallCount);
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

void ANumberBaseballPlayerController::Server_JoinGame_Implementation()
{
	if (ANumberBaseballGameState* NumberBaseballGameState = Cast<ANumberBaseballGameState>(GetWorld()->GetGameState()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Index TEST : %d"), GetWorld()->GetNumPlayerControllers() - 1);
		// NumberBaseballGameState->JoinGame(GetWorld()->GetNumPlayerControllers() - 1);
	}
}

bool ANumberBaseballPlayerController::Server_JoinGame_Validate()
{
	return true;
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
		NumberBaseballPlayerState->GameReady();
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
		if (const ANumberBaseballPlayerState* NumberBaseballPlayerState = GetPlayerState<ANumberBaseballPlayerState>())
		{
			// GameMode->GotInputText(NumberBaseballPlayerState->GetReplicatedPlayerID(), InputText);
		}
	}
}

bool ANumberBaseballPlayerController::Server_SetInputText_Validate(const FString& InputText)
{
	// 필요시 유효성 체크 로직 구현
	return true;
}
