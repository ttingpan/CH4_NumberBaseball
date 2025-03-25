#include "ChatWidget.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"

void UChatWidget::InitWidget(const bool bIsMyChat, const FString& InPlayerName, const FString& InInputText) const
{
	if (bIsMyChat)
	{
		if (UBorder* Border = Cast<UBorder>(PlayerName->GetParent()))
		{
			Border->SetBrushColor(FColor::Green);
		}
	}
	
	PlayerName->SetText(FText::FromString(InPlayerName));
	InputText->SetText(FText::FromString(InInputText));
	StrikeText->SetText(FText::FromString(TEXT("")));
	BallText->SetText(FText::FromString(TEXT("")));

	StrikeText->GetParent()->SetVisibility(ESlateVisibility::Hidden);
	BallText->GetParent()->SetVisibility(ESlateVisibility::Hidden);
	OutTextBorder->SetVisibility(ESlateVisibility::Hidden);
	HitTextBorder->SetVisibility(ESlateVisibility::Hidden);
}

void UChatWidget::UpdateResult(const int32 StrikeCount, const int32 BallCount) const
{
	if (StrikeCount == 0 && BallCount == 0)
	{
		OutTextBorder->SetVisibility(ESlateVisibility::Visible);
	}
	else if (StrikeCount == 3)
	{
		HitTextBorder->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		StrikeText->SetText(FText::FromString(FString::Printf(TEXT("%iS"), StrikeCount)));
		BallText->SetText(FText::FromString(FString::Printf(TEXT("%iB"), BallCount)));
		
		StrikeText->GetParent()->SetVisibility(ESlateVisibility::Visible);
		BallText->GetParent()->SetVisibility(ESlateVisibility::Visible);
	}
}
