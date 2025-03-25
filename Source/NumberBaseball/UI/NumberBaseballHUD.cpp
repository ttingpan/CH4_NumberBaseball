#include "NumberBaseballHUD.h"

#include "GameOverWidget.h"
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
		}

		if (GameOverWidgetClass && !GameOverWidget)
		{
			GameOverWidget = CreateWidget<UGameOverWidget>(GetOwningPlayerController(), GameOverWidgetClass);
		}
	}
}

void ANumberBaseballHUD::JoinGame(const int32 Index, const int32 WinScore) const
{
	if (JoinGameWidget->IsInViewport())
	{
		JoinGameWidget->RemoveFromParent();
	}

	if (!MainWidget->IsInViewport())
	{
		MainWidget->InitWidget(PlayerSlotWidgetClass, Index, WinScore);
		MainWidget->AddToViewport();
	}
}

void ANumberBaseballHUD::AddChatWidget(const bool bIsMyChat, const FString& PlayerName, const FString& InputText) const
{
	MainWidget->AddChatWidget(bIsMyChat, ChatWidgetClass, PlayerName, InputText);
}

void ANumberBaseballHUD::AddChatRoundNotifyWidget(const int32 CurrentRound, const bool bIsStart) const
{
	if (MainWidget)
	{
		MainWidget->AddChatRoundNotifyWidget(ChatRoundNotifyWidgetClass, CurrentRound, bIsStart);
	}
}

void ANumberBaseballHUD::ShowGameOverWidget(const bool bIsWin, const APlayerState* WinnerPlayerState) const
{
	if (GameOverWidget)
	{
		GameOverWidget->InitWidget(bIsWin, WinnerPlayerState);
		GameOverWidget->AddToViewport();
	}
}
