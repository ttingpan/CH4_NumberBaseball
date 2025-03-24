#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatRoundNotifyWidget.generated.h"

UCLASS(Abstract)
class NUMBERBASEBALL_API UChatRoundNotifyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 위젯 초기화
	void InitWidget(const int32 CurrentRound, bool bIsStart) const;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RoundNotifyText;
};
