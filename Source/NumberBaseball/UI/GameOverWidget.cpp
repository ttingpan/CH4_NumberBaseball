#include "GameOverWidget.h"

#include "Components/TextBlock.h"
#include "Player/NumberBaseballPlayerState.h"

void UGameOverWidget::InitWidget(const bool bIsWin, const APlayerState* WinnerPlayerState) const
{
	ResultText->SetText(FText::FromString(bIsWin ? TEXT("승리") : TEXT("패배")));

	FString WinnerPlayerName = TEXT("Unknown");
	
	if (WinnerPlayerState)
	{
		WinnerPlayerName = WinnerPlayerState->GetPlayerName();
	}
	WinnerPlayerText
			->SetText(FText::FromString(FString::Printf(TEXT("승자 : %s"), *WinnerPlayerName)));
}
