#include "BaseBallGamePlayerController.h"

#include "BaseBallGame/BaseBallGameGameMode.h"
#include "BaseBallGame/FunctionLibrary/ComparingNumbersLib.h"
#include "BaseBallGame/UI/ChatWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void ABaseBallGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// UI 모드 설정
	SetInputMode(FInputModeUIOnly());
	// 마우스 커서 보이기
	bShowMouseCursor = true;
	
	if (GetRemoteRole() != ROLE_None)
	{
		// 0.5초 딜레이
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&ABaseBallGamePlayerController::BindToWidgetDelegate,
			0.5f,
			false
		);
	}
}

void ABaseBallGamePlayerController::GotBroadcastMessage_Implementation(const FString& Message) const
{
	// 서버 스스로 호출하지 않도록 방어
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("GotBroadcastMessage: %s"), *Message);
	}
}

void ABaseBallGamePlayerController::BindToWidgetDelegate()
{
	// 채팅 위젯 찾기
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, Widgets, UChatWidget::StaticClass(), true);

	if (Widgets.Num() > 0)
	{
		if (UChatWidget* ChatWidget = Cast<UChatWidget>(Widgets[0]))
		{
			// 커밋 델리게이트 바인딩
			ChatWidget->OnInputCommitted.AddDynamic(this, &ABaseBallGamePlayerController::SetMessageToUserController);
		}
	}
}

void ABaseBallGamePlayerController::OnLoginWithID_Implementation(const FString& InUserID)
{
	UserID = InUserID;
}

void ABaseBallGamePlayerController::SetMessageToUserController_Implementation(const FString& InputText)
{
	if (const ABaseBallGameGameMode* GameMode = Cast<ABaseBallGameGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->GotMessageFromClient(UserID, InputText);
	}
}
