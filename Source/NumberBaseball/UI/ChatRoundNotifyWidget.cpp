#include "ChatRoundNotifyWidget.h"

#include "Components/TextBlock.h"

void UChatRoundNotifyWidget::InitWidget(const int32 CurrentRound, const bool bIsStart) const
{
	const FString Text = TEXT("Round ") + FString::FromInt(CurrentRound) + (bIsStart ? TEXT(" 시작") : TEXT(" 종료"));
	RoundNotifyText->SetText(FText::FromString(Text));
}
