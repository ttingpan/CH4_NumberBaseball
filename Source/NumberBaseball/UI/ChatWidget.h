#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ChatWidget.generated.h"

class UTextBlock;
class UEditableTextBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputCommitted, const FString&, InputText);

UCLASS()
class NUMBERBASEBALL_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOtherUserName(const FString& OtherUserID) const;

	UFUNCTION(BlueprintCallable, Category = "Chat|Widget")
	void InitWidget(int32 InTargetNumberLength);
	
	// 커밋 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Chat|Widget")
	FOnInputCommitted OnInputCommitted;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* InputTextBox;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HelpMessage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* OtherUserName;

private:
	UFUNCTION()
	void HandleTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
	UFUNCTION()
	void HandleTextChanged(const FText& Text);

	// 목표 글자 수
	int32 TargetNumberLength = 0;
};
