#include "ChatWidget.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"

void UChatWidget::InitWidget(const FString& InPlayerName, const FString& InInputText) const
{
	PlayerName->SetText(FText::FromString(InPlayerName));
	InputText->SetText(FText::FromString(InInputText));
	StrikeText->SetText(FText::FromString(TEXT("")));
	BallText->SetText(FText::FromString(TEXT("")));

	OutTextBorder->SetVisibility(ESlateVisibility::Hidden);
}

void UChatWidget::UpdateResult(const int32 StrikeCount, const int32 BallCount) const
{
	if (StrikeCount == 0 && BallCount == 0)
	{
		OutTextBorder->SetVisibility(ESlateVisibility::Visible);

		StrikeText->GetParent()->SetVisibility(ESlateVisibility::Hidden);
		BallText->GetParent()->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		StrikeText->SetText(FText::FromString(FString::Printf(TEXT("%iS"), StrikeCount)));
		BallText->SetText(FText::FromString(FString::Printf(TEXT("%iB"), BallCount)));
	}
}
