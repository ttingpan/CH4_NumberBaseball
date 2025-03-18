#include "MainWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
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

void UMainWidget::GameStarted(const int32 InTargetNumberLength)
{
	TargetNumberLength = InTargetNumberLength;
	InputTextBox->SetIsEnabled(true);
	SetHelpMessage();
	ReadyButton->SetVisibility(ESlateVisibility::Hidden);
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

	ReadyButton->OnClicked.AddDynamic(this, &UMainWidget::OnReadyButtonClicked);

	if (ANumberBaseballPlayerController* NumberBaseballPlayerController = Cast<ANumberBaseballPlayerController>(
		GetOwningPlayer()))
	{
		NumberBaseballPlayerController->SetMainWidget(this);
	}
}

void UMainWidget::SetReadyButtonText()
{
	if (UTextBlock* ReadyButtonText = Cast<UTextBlock>(ReadyButton->GetChildAt(0)))
	{
		bIsReady = !bIsReady;
		
		ReadyButtonText->SetText(
			FText::FromString(bIsReady ? TEXT("준비 완료") : TEXT("게임 준비")));
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
	// 버튼 텍스트 변경
	SetReadyButtonText();

	// 서버로 게임 준비 전달
	OnReadyButtonClickedDelegate.Broadcast();
}
