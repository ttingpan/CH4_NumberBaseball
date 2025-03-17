#include "NumberBaseballHUD.h"

#include "MainWidget.h"
#include "Blueprint/UserWidget.h"

void ANumberBaseballHUD::BeginPlay()
{
	Super::BeginPlay();

	UMainWidget* MainWidget = CreateWidget<UMainWidget>(GetOwningPlayerController(), MainWidgetClass);
	MainWidget->InitWidget();
	MainWidget->AddToViewport();
}
