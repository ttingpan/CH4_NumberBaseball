#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MainWidget.generated.h"

class UButton;
class ANumberBaseballHUD;
class UTextBlock;
class UEditableTextBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputCommitted, const FString&, InputText);

UCLASS()
class NUMBERBASEBALL_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 상대 이름 설정
	void SetOtherPlayerName(const FString& InOtherPlayerName) const;

	// 입력 가능 여부 변경
	void GameStarted(const int32& InTargetNumberLength);

	// 도움말 메세지 설정
	void SetHelpMessage() const;
	void SetHelpMessage(const FString& InHelpMessage) const;

	// 위젯 초기화
	void InitWidget();

	// 커밋 델리게이트
	UPROPERTY()
	FOnInputCommitted OnInputCommitted;

	// 목표 글자 수 반환
	FORCEINLINE int32 GetTargetNumberLength() const { return TargetNumberLength; }

	// 준비 버튼 텍스트 설정
	void SetReadyButtonText(const bool bIsReady) const;

protected:
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* InputTextBox;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HelpMessage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* OtherPlayerName;
	UPROPERTY(meta = (BindWidget))
	UButton* ReadyButton;

private:
	UFUNCTION()
	void HandleTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void HandleTextChanged(const FText& Text);
	UFUNCTION()
	void HandleReadyButtonClicked();

	// 유효한 입력인지 확인
	bool IsValidInput(const FString& InputText, const FString& LastChar) const;

	// 목표 글자 수
	int32 TargetNumberLength = 0;
};
