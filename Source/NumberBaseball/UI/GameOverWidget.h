#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

class ANumberBaseballPlayerState;
class UTextBlock;

UCLASS(Abstract)
class NUMBERBASEBALL_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 위젯 초기화
	void InitWidget(const bool bIsWin, const APlayerState* WinnerPlayerState) const;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ResultText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WinnerPlayerText;
};
