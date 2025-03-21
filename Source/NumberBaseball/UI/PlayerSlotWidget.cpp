#include "PlayerSlotWidget.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"

void UPlayerSlotWidget::InitWidget(const int32 Index)
{
	bIsMySlotWidget = Index == 0;
	OutLineBorder->SetBrushColor(bIsMySlotWidget ? FColor::Green : FColor::White);
	PlayerNameText->SetText(FText::FromString(""));
	PlayerScoreText->SetText(FText::FromString("0"));

	SetRenderOpacity(bIsMySlotWidget ? 1.0f : 0.5f);
}

void UPlayerSlotWidget::TurnUpdate(const bool bIsMyTurn) const
{
	const FColor DefaultColor = bIsMySlotWidget ? FColor::Green : FColor::White;
	OutLineBorder->SetBrushColor(bIsMyTurn ? FColor::Blue : DefaultColor);
}

void UPlayerSlotWidget::SetPlayerName(const FString& InPlayerName)
{
	PlayerNameText->SetText(FText::FromString(InPlayerName));

	SetRenderOpacity(1.0f);
}

void UPlayerSlotWidget::UpdateScore(const int32 Score) const
{
	PlayerScoreText->SetText(FText::FromString(FString::FromInt(Score)));
}
