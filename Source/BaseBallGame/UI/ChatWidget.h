#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ChatWidget.generated.h"

class UTextBlock;
class UEditableTextBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputCommitted, const FString&, InputText);

UCLASS()
class BASEBALLGAME_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Chat|Widget")
	void InitWidget();

	// 커밋 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Chat|Widget")
	FOnInputCommitted OnInputCommitted;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UEditableTextBox* InputTextBox;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ErrorMessageText;

private:
	UFUNCTION()
	void HandleTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
	UFUNCTION()
	void HandleTextChanged(const FText& Text);
};
