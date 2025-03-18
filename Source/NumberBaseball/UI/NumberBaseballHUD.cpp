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
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("JoinGameWidgetClass가 유효하지 않습니다."));
	}
}

void ANumberBaseballHUD::JoinGame() const
{
	if (JoinGameWidget)
	{
		JoinGameWidget->RemoveFromParent();
	}
	
	if (MainWidgetClass)
	{
		UMainWidget* MainWidget = CreateWidget<UMainWidget>(GetOwningPlayerController(), MainWidgetClass);
		MainWidget->InitWidget();
		MainWidget->AddToViewport();
	}
}
