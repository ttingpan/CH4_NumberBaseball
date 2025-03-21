#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MainWidget.generated.h"

class UVerticalBox;
class UPlayerSlotWidget;
class UChatWidget;
class UScrollBox;
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
	// 플레이어 참가
	void SetPlayerName(int32 Index, const FString& PlayerName);
	// 특정 플레이어 슬롯 위젯 업데이트
	void UpdatePlayerSlotWidgetByIndex(int32 Index, bool bIsMyTurn);

	// 게임 시작 준비
	void PrepareStartTurn(int32 InTargetNumberLength);
	// 턴 시작
	void TurnStarted() const;
	// 턴 종료
	void TurnEnded(bool bIsAuto) const;
	// 타이머 텍스트 업데이트
	void UpdateTimerText(const FString& InTimerText) const;

	// 도움말 메세지 설정
	void SetHelpMessage() const;
	void SetHelpMessage(const FString& InHelpMessage) const;
	// 위젯 초기화
	void InitWidget(const TSubclassOf<UPlayerSlotWidget>& PlayerSlotWidgetClass);
	// 준비 버튼 초기화
	void InitReadyButton(bool bIsHost);
	// 준비 버튼 텍스트 설정
	void SetReadyButtonText(bool bIsReady) const;
	// 게임 시작 버튼 활성화
	void SetReadyButtonIsEnabled(bool bIsReady) const;

	// 채팅 위젯 추가
	void AddChatWidget(const TSubclassOf<UChatWidget>& ChatWidgetClass, const FString& InPlayerName,
	                   const FString& InInputText);
	// 판정 결과 업데이트
	void UpdateResult(int32 StrikeCount, int32 BallCount) const;

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
	UButton* ReadyButton;
	// UPROPERTY(meta = (BindWidget))
	// UTextBlock* RoundText;
	// UPROPERTY(meta = (BindWidget))
	// UTextBlock* TurnText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerText;
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBox;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayerSlotWidgetVerticalBox;

	// 모든 플레이어 슬롯 위젯 목록
	TArray<TObjectPtr<UPlayerSlotWidget>> PlayerSlotWidgets;

	// 목표 글자 수
	int32 TargetNumberLength = 0;

	UPROPERTY()
	UChatWidget* LastChatWidget;
};
