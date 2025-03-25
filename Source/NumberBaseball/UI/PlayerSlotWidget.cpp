#include "PlayerSlotWidget.h"

#include "Components/Border.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerSlotWidget::InitWidget(const bool bInIsMySlotWidget, const int32 InWinScore)
{
	bIsMySlotWidget = bInIsMySlotWidget;
	PlayerNameBorder->SetBrushColor(bIsMySlotWidget ? FColor::Green : FColor::White);
	PlayerNameText->SetText(FText::FromString(""));
	WinScore = InWinScore;
	PlayerScoreProgressBar->SetPercent(0.0f);
	PlayerScoreText->SetText(FText::FromString("0"));
	ReadyTextBorder->SetVisibility(ESlateVisibility::Hidden);

	SetRenderOpacity(bIsMySlotWidget ? 1.0f : 0.5f);
}

void UPlayerSlotWidget::TurnUpdate(const bool bIsMyTurn) const
{
	const FColor DefaultColor = bIsMySlotWidget ? FColor::Green : FColor::White;
	PlayerNameBorder->SetBrushColor(bIsMyTurn ? FColor::Blue : DefaultColor);
	PlayerNameText->SetColorAndOpacity(bIsMyTurn ? FColor::White : FColor::Black);
}

void UPlayerSlotWidget::SetPlayerName(const FString& InPlayerName)
{
	PlayerNameText->SetText(FText::FromString(InPlayerName));

	SetRenderOpacity(1.0f);
}

void UPlayerSlotWidget::UpdateScore(const int32 Score) const
{
	PlayerScoreText->SetText(FText::FromString(FString::FromInt(Score)));
	PlayerScoreProgressBar->SetPercent(static_cast<float>(Score) / static_cast<float>(WinScore));
}

void UPlayerSlotWidget::SetVisibilityReadyTextBorder(const bool bIsVisible) const
{
	ReadyTextBorder->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
