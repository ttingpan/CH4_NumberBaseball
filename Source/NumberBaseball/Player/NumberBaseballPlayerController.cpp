#include "NumberBaseballPlayerController.h"

#include "NumberBaseballPlayerState.h"
#include "GameFramework/GameUserSettings.h"
#include "NumberBaseball/NumberBaseballGameMode.h"
#include "UI/MainWidget.h"

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

void ANumberBaseballPlayerController::SetOtherPlayerName(const FString& OtherPlayerName) const
{
	MainWidget->SetOtherPlayerName(OtherPlayerName);
}

void ANumberBaseballPlayerController::GameStarted_Implementation(const int32& TargetNumberLength)
{
	if (MainWidget)
	{
		MainWidget->GameStarted(TargetNumberLength);
		MainWidget->OnInputCommitted.AddDynamic(this, &ANumberBaseballPlayerController::Server_SetInputText);
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
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client_GotInputText: %s"), *Message);
	}
}

void ANumberBaseballPlayerController::Server_SetInputText_Implementation(const FString& InputText)
{
	if (const ANumberBaseballGameMode* GameMode = Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->Server_GotInputText(PlayerState->GetPlayerName(), InputText);
	}
}

bool ANumberBaseballPlayerController::Server_SetInputText_Validate(const FString& InputText)
{
	// 필요시 유효성 체크 로직 구현
	return true;
}
