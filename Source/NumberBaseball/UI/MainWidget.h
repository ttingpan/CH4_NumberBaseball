#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MainWidget.generated.h"

class UGameOverWidget;
class UChatRoundNotifyWidget;
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
	// 위젯 초기화
	void InitWidget(const TSubclassOf<UPlayerSlotWidget>& PlayerSlotWidgetClass, int32 Index, int32 WinScore);
	// 준비 버튼 초기화
	void InitReadyButton(bool bIsHost);

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
	// 라운드 텍스트 업데이트
	void UpdateRoundText(int32 CurrentRound) const;
	// 턴 텍스트 업데이트
	void UpdateTurnText(int32 CurrentTurn) const;

	// 도움말 메세지 설정
	void SetHelpMessage() const;
	void SetHelpMessage(const FString& InHelpMessage) const;
	// 준비 버튼 텍스트 설정
	void SetReadyButtonText(bool bIsReady) const;
	// 게임 시작 버튼 활성화
	void SetReadyButtonIsEnabled(bool bIsReady) const;
	
	// 채팅 위젯 추가
	void AddChatWidget(bool bIsMyChat, const TSubclassOf<UChatWidget>& ChatWidgetClass,
	                   const FString& InPlayerName, const FString& InInputText);
	// 판정 결과 업데이트
	void UpdateResult(int32 StrikeCount, int32 BallCount) const;
	// 점수 업데이트
	void UpdateScore(int32 Index, const int32 Score);
	// 준비 완료 표시 설정
	void SetVisibilityReadyTextBorder(int32 Index, bool bIsVisible) const;

	// 채팅 라운드 알림 위젯 추가
	void AddChatRoundNotifyWidget(const TSubclassOf<UChatRoundNotifyWidget>& ChatRoundNotifyWidgetClass, const int32 CurrentRound, const bool bIsStart);
	
	// 커밋 델리게이트
	UPROPERTY()
	FOnInputCommitted OnInputCommittedDelegate;
	// 준비 버튼 델리게이트
	UPROPERTY()
	FOnReadyButtonClicked OnReadyButtonClickedDelegate;

protected:
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* InputTextBox;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HelpMessage;
	UPROPERTY(meta = (BindWidget))
	UButton* ReadyButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoundText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TurnText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerText;
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBox;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayerSlotWidgetVerticalBox;
	
private:
	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void OnTextChanged(const FText& Text);
	UFUNCTION()
	void OnReadyButtonClicked();

	// 모든 플레이어 슬롯 위젯 목록
	TArray<TObjectPtr<UPlayerSlotWidget>> PlayerSlotWidgets;

	// 목표 글자 수
	int32 TargetNumberLength = 0;

	UPROPERTY()
	TObjectPtr<UChatWidget> LastChatWidget;
	UPROPERTY()
	TObjectPtr<UChatRoundNotifyWidget> LastChatRoundNotifyWidget;

	FTimerHandle HelpMessageColorTimerHandle;
};
