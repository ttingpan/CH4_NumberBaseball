#include "NumberBaseballHUD.h"

#include "MainWidget.h"
#include "JoinGameWidget.h"
#include "Blueprint/UserWidget.h"

void ANumberBaseballHUD::BeginPlay()
{
	Super::BeginPlay();

	if (JoinGameWidgetClass)
	{
		JoinGameWidget = CreateWidget<UJoinGameWidget>(GetOwningPlayerController(),
		                                                                JoinGameWidgetClass);
		JoinGameWidget->InitWidget();
		JoinGameWidget->AddToViewport();

		if (MainWidgetClass)
		{
			MainWidget = CreateWidget<UMainWidget>(GetOwningPlayerController(), MainWidgetClass);
			MainWidget->InitWidget(PlayerSlotWidgetClass);
		}
	}
}

void ANumberBaseballHUD::JoinGame() const
{
	if (JoinGameWidget)
	{
		JoinGameWidget->RemoveFromParent();
	}
	
	if (MainWidget)
	{
		MainWidget->AddToViewport();
	}
}

void ANumberBaseballHUD::AddChatWidget(const FString& PlayerName, const FString& InputText) const
{
	MainWidget->AddChatWidget(ChatWidgetClass, PlayerName, InputText);
}
