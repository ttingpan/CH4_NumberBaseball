#include "NumberBaseballPlayerController.h"

#include "NumberBaseball/NumberBaseballGameMode.h"
#include "NumberBaseball/FunctionLibrary/ComparingNumbersLib.h"
#include "NumberBaseball/UI/ChatWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void ANumberBaseballPlayerController::BeginPlay()
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
			&ANumberBaseballPlayerController::BindToWidgetDelegate,
			0.5f,
			false
		);
	}
}

void ANumberBaseballPlayerController::GotBroadcastMessage_Implementation(const FString& Message) const
{
	// 서버 스스로 호출하지 않도록 방어
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("GotBroadcastMessage: %s"), *Message);
	}
}

void ANumberBaseballPlayerController::BindToWidgetDelegate()
{
	// 채팅 위젯 찾기
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, Widgets, UChatWidget::StaticClass(), true);

	if (Widgets.Num() > 0)
	{
		if (UChatWidget* ChatWidget = Cast<UChatWidget>(Widgets[0]))
		{
			// 커밋 델리게이트 바인딩
			ChatWidget->OnInputCommitted.AddDynamic(this, &ANumberBaseballPlayerController::SetMessageToUserController);
		}
	}
}

void ANumberBaseballPlayerController::OnLoginWithID_Implementation(const FString& InUserID)
{
	UserID = InUserID;
}

void ANumberBaseballPlayerController::SetMessageToUserController_Implementation(const FString& InputText)
{
	if (const ANumberBaseballGameMode* GameMode = Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->GotMessageFromClient(UserID, InputText);
	}
}
