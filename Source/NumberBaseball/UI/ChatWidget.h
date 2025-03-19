#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatWidget.generated.h"

class UBorder;
class UTextBlock;

UCLASS()
class NUMBERBASEBALL_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitWidget(const FString& InPlayerName, const FString& InInputText) const;

	void UpdateResult(int32 StrikeCount, int32 BallCount) const;
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InputText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StrikeText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BallText;
	UPROPERTY(meta = (BindWidget))
	UBorder* OutTextBorder;
};
