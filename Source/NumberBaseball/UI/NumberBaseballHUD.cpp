#include "NumberBaseballHUD.h"

#include "ChatRoundNotifyWidget.h"
#include "MainWidget.h"
#include "JoinGameWidget.h"
#include "Blueprint/UserWidget.h"

void ANumberBaseballHUD::BeginPlay()
{
	Super::BeginPlay();

	if (JoinGameWidgetClass && !JoinGameWidget)
	{
		JoinGameWidget = CreateWidget<UJoinGameWidget>(GetOwningPlayerController(),
		                                               JoinGameWidgetClass);
		JoinGameWidget->InitWidget();
		JoinGameWidget->AddToViewport();

		if (MainWidgetClass && !MainWidget)
		{
			MainWidget = CreateWidget<UMainWidget>(GetOwningPlayerController(), MainWidgetClass);
			MainWidget->InitWidget(PlayerSlotWidgetClass);
		}
	}
}

void ANumberBaseballHUD::JoinGame() const
{
	if (JoinGameWidget->IsInViewport())
	{
		JoinGameWidget->RemoveFromParent();
	}

	if (!MainWidget->IsInViewport())
	{
		MainWidget->AddToViewport();
	}
}

void ANumberBaseballHUD::AddChatWidget(const FString& PlayerName, const FString& InputText) const
{
	MainWidget->AddChatWidget(ChatWidgetClass, PlayerName, InputText);
}

void ANumberBaseballHUD::AddChatRoundNotifyWidget(const int32 CurrentRound, const bool bIsStart) const
{
	if (MainWidget)
	{
		MainWidget->AddChatRoundNotifyWidget(ChatRoundNotifyWidgetClass, CurrentRound, bIsStart);
	}
}
