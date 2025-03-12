#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ChatWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageCommitted, const FString&, Message);

class UEditableTextBox;

UCLASS()
class BASEBALLGAME_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Chat|Widget")
	void InitWidget();

	// 커밋 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Chat|Widget")
	FOnMessageCommitted OnMessageCommitted;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UEditableTextBox* ChatTextBox;

private:
	UFUNCTION()
	void HandleTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
