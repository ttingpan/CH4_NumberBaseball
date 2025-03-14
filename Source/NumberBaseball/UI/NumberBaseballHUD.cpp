#include "NumberBaseballHUD.h"

#include "NumberBaseballGameState.h"
#include "Blueprint/UserWidget.h"
#include "UI/ChatWidget.h"

void ANumberBaseballHUD::Init()
{
	if (const ANumberBaseballGameState* NumberBaseballGameState = Cast<ANumberBaseballGameState>(GetWorld()->GetGameState()))
	{
		ChatWidget = CreateWidget<UChatWidget>(GetOwningPlayerController(), ChatWidgetClass);
		ChatWidget->InitWidget(NumberBaseballGameState->GetTargetNumberLength());
		ChatWidget->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameState가 유효하지 않습니다."));
	}
}
