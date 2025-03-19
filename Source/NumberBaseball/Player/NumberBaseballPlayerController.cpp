// PRAGMA_DISABLE_OPTIMIZATION

#include "NumberBaseballPlayerController.h"

#include "NumberBaseballPlayerState.h"
#include "GameFramework/GameUserSettings.h"
#include "NumberBaseball/NumberBaseballGameMode.h"
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

void ANumberBaseballPlayerController::Client_OnTurnStarted_Implementation()
{
	if (MainWidget)
	{
		MainWidget->TurnStarted();
	}
}

void ANumberBaseballPlayerController::Client_InitReadyButton_Implementation(const bool bIsHost)
{
	if (MainWidget)
	{
		MainWidget->InitReadyButton(bIsHost);
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

void ANumberBaseballPlayerController::Client_AddChatWidget_Implementation(const FString& PlayerName,
	const FString& InputText) const
{
	if (GetHUD())
	{
		if (const ANumberBaseballHUD* NumberBaseballHUD = Cast<ANumberBaseballHUD>(GetHUD()))
		{
			NumberBaseballHUD->AddChatWidget(PlayerName, InputText);
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

void ANumberBaseballPlayerController::Client_SetOtherPlayerName_Implementation(const FString& OtherPlayerName) const
{
	if (MainWidget)
	{
		MainWidget->SetOtherPlayerName(OtherPlayerName);
	}
}

void ANumberBaseballPlayerController::Server_JoinGame_Implementation(const FString& PlayerName)
{
	if (ANumberBaseballGameMode* NumberBaseballGameMode = Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode()))
	{
		NumberBaseballGameMode->JoinGame(this, PlayerName);
	}
}

bool ANumberBaseballPlayerController::Server_JoinGame_Validate(const FString& PlayerName)
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

void ANumberBaseballPlayerController::Client_GotInputText_Implementation(const FString& Message) const
{
	// 서버 스스로 호출하지 않도록 방어
	UE_LOG(LogTemp, Warning, TEXT("Client_GotInputText: %s"), *Message);
}

void ANumberBaseballPlayerController::Server_SetInputText_Implementation(const FString& InputText)
{
	if (ANumberBaseballGameMode* GameMode = Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->Server_GotInputText(PlayerState->GetPlayerName(), InputText);
	}
}

bool ANumberBaseballPlayerController::Server_SetInputText_Validate(const FString& InputText)
{
	// 필요시 유효성 체크 로직 구현
	return true;
}
