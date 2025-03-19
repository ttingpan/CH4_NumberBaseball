#include "MainWidget.h"

#include "ChatWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "FunctionLibrary/InputValidationLib.h"
#include "Player/NumberBaseballPlayerController.h"

void UMainWidget::SetOtherPlayerName(const FString& InOtherPlayerName) const
{
	if (OtherPlayerName)
	{
		FString UserName = "상대: " + InOtherPlayerName;
		OtherPlayerName->SetText(FText::FromString(InOtherPlayerName));
	}
}

void UMainWidget::PrepareStartTurn(const int32 InTargetNumberLength)
{
	TargetNumberLength = InTargetNumberLength;
	ReadyButton->SetVisibility(ESlateVisibility::Hidden);
}

void UMainWidget::TurnStarted() const
{
	InputTextBox->SetIsEnabled(true);
	SetHelpMessage();
}

void UMainWidget::TurnEnded(const bool bIsAuto) const
{
	InputTextBox->SetIsEnabled(false);

	const FString HelpMessageText = bIsAuto ? TEXT("상대 턴 입니다. 다음 턴을 기다리세요.") : TEXT("");
	SetHelpMessage(HelpMessageText);
}

void UMainWidget::UpdateTimerText(const FString& InTimerText) const
{
	TimerText->SetText(FText::FromString(InTimerText));
}

void UMainWidget::SetHelpMessage() const
{
	// 도움말 메세지 설정
	const FString HelpMessageText = FString::Printf(TEXT("숫자 %d개를 입력하세요."), TargetNumberLength);
	SetHelpMessage(HelpMessageText);
}

void UMainWidget::SetHelpMessage(const FString& InHelpMessage) const
{
	if (HelpMessage)
	{
		HelpMessage->SetText(FText::FromString(InHelpMessage));
	}
}

void UMainWidget::InitWidget()
{
	// 커밋 이벤트 바인딩
	InputTextBox->OnTextCommitted.AddDynamic(this, &UMainWidget::OnTextCommitted);
	// 텍스트 변경 이벤트 바인딩
	InputTextBox->OnTextChanged.AddDynamic(this, &UMainWidget::OnTextChanged);

	InputTextBox->SetIsEnabled(false);

	OtherPlayerName->SetText(FText::FromString(""));
	HelpMessage->SetText(FText::FromString(""));
	TimerText->SetText(FText::FromString(""));

	if (ANumberBaseballPlayerController* NumberBaseballPlayerController = Cast<ANumberBaseballPlayerController>(
		GetOwningPlayer()))
	{
		NumberBaseballPlayerController->SetMainWidget(this);
	}
}

void UMainWidget::InitReadyButton(const bool bIsHost)
{
	if (ReadyButton && ReadyButton->GetChildAt(0))
	{
		// 텍스트 설정
		if (UTextBlock* ReadyButtonText = Cast<UTextBlock>(ReadyButton->GetChildAt(0)))
		{
			ReadyButtonText->SetText(
				FText::FromString(bIsHost ? TEXT("게임 시작") : TEXT("게임 준비")));
		}

		// 활성화 설정
		ReadyButton->SetIsEnabled(!bIsHost);

		// 이벤트 바인딩
		ReadyButton->OnClicked.AddDynamic(this, &UMainWidget::OnReadyButtonClicked);
	}
}

void UMainWidget::SetReadyButtonText(const bool bIsReady) const
{
	if (UTextBlock* ReadyButtonText = Cast<UTextBlock>(ReadyButton->GetChildAt(0)))
	{
		ReadyButtonText->SetText(
			FText::FromString(bIsReady ? TEXT("준비 완료") : TEXT("게임 준비")));
	}
}

void UMainWidget::SetReadyButtonIsEnabled(const bool bIsReady) const
{
	if (ReadyButton)
	{
		ReadyButton->SetIsEnabled(bIsReady);
	}
}

void UMainWidget::AddChatWidget(const TSubclassOf<UChatWidget>& ChatWidgetClass, const FString& InPlayerName,
                                const FString& InInputText)
{
	LastChatWidget = CreateWidget<UChatWidget>(this, ChatWidgetClass);
	if (LastChatWidget)
	{
		LastChatWidget->InitWidget(InPlayerName, InInputText);
		
		if (ScrollBox)
		{
			ScrollBox->AddChild(LastChatWidget);
		}
	}
}

void UMainWidget::UpdateResult(const int32 StrikeCount, const int32 BallCount) const
{
	if (LastChatWidget)
	{
		LastChatWidget->UpdateResult(StrikeCount, BallCount);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UMainWidget::OnTextCommitted(const FText& Text, const ETextCommit::Type CommitMethod)
{
	// 엔터를 눌렀을 경우만 실행
	if (CommitMethod == ETextCommit::OnEnter)
	{
		const FString InputText = Text.ToString();

		// 입력한 글자 수가 목표 글자 수와 같을 경우만 실행
		if (InputText.Len() == TargetNumberLength)
		{
			// 서버로 입력한 값 전달
			OnInputCommittedDelegate.Broadcast(InputText);

			// 입력 창 초기화
			InputTextBox->SetText(FText::GetEmpty());
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UMainWidget::OnTextChanged(const FText& Text)
{
	FString InputText = Text.ToString();

	// 입력 검증 => 1-9숫자만 입력 가능, 중복 불가능
	if (!UInputValidationLib::IsValidInputText(InputText, TargetNumberLength, FRegexPattern(TEXT("^[1-9]*$")), false))
	{
		// 입력 제거(마지막 문자 제거)
		InputText = InputText.LeftChop(1);
	}

	InputTextBox->SetText(Text.FromString(InputText));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UMainWidget::OnReadyButtonClicked()
{
	// 서버로 게임 준비 전달
	OnReadyButtonClickedDelegate.Broadcast();
}
