#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MainWidget.generated.h"

class ATurnManager;
class UButton;
class ANumberBaseballHUD;
class UTextBlock;
class UEditableTextBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputCommitted, const FString&, InputText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReadyButtonClicked);

UCLASS(Abstract)
class NUMBERBASEBALL_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 상대 이름 설정
	void SetOtherPlayerName(const FString& InOtherPlayerName) const;
	// 게임 시작
	void GameStarted(int32 InTargetNumberLength);

	// 타이머 텍스트 업데이트
	void UpdateTimerText(const FString& InTimerText) const;

	// 도움말 메세지 설정
	void SetHelpMessage() const;
	void SetHelpMessage(const FString& InHelpMessage) const;
	// 위젯 초기화
	void InitWidget();
	// 준비 버튼 텍스트 설정
	void SetReadyButtonText();

	// 커밋 델리게이트
	UPROPERTY()
	FOnInputCommitted OnInputCommittedDelegate;
	// 준비 버튼 델리게이트
	UPROPERTY()
	FOnReadyButtonClicked OnReadyButtonClickedDelegate;
	
private:
	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void OnTextChanged(const FText& Text);
	UFUNCTION()
	void OnReadyButtonClicked();

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* InputTextBox;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HelpMessage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* OtherPlayerName;
	UPROPERTY(meta = (BindWidget))
	UButton* ReadyButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerText;
	
	// 목표 글자 수
	int32 TargetNumberLength = 0;

	// 준비 여부
	bool bIsReady = false;
};
