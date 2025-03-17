#include "MainWidget.h"

#include "NumberBaseballPlayerState.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Player/NumberBaseballPlayerController.h"

void UMainWidget::SetOtherPlayerName(const FString& InOtherPlayerName) const
{
	if (OtherPlayerName)
	{
		FString UserName = "상대: " + InOtherPlayerName;
		OtherPlayerName->SetText(FText::FromString(InOtherPlayerName));
	}
}

void UMainWidget::GameStarted(const int32& InTargetNumberLength)
{
	TargetNumberLength = InTargetNumberLength;

	InputTextBox->SetIsEnabled(true);
	SetHelpMessage();
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
	InputTextBox->OnTextCommitted.AddDynamic(this, &UMainWidget::HandleTextCommitted);
	// 텍스트 변경 이벤트 바인딩
	InputTextBox->OnTextChanged.AddDynamic(this, &UMainWidget::HandleTextChanged);

	InputTextBox->SetIsEnabled(false);

	OtherPlayerName->SetText(FText::FromString(""));
	HelpMessage->SetText(FText::FromString(""));

	ReadyButton->OnClicked.AddDynamic(this, &UMainWidget::HandleReadyButtonClicked);

	if (ANumberBaseballPlayerController* NumberBaseballPlayerController = Cast<ANumberBaseballPlayerController>(
		GetOwningPlayer()))
	{
		NumberBaseballPlayerController->SetMainWidget(this);
	}
}

void UMainWidget::SetReadyButtonText(const bool bIsReady) const
{
	if (UTextBlock* ReadyButtonText = Cast<UTextBlock>(ReadyButton->GetChildAt(0)))
	{
		ReadyButtonText->SetText(
			FText::FromString(bIsReady ? TEXT("준비 완료") : TEXT("게임 준비")));

		UE_LOG(LogTemp, Warning, TEXT("TEST!!!"));
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UMainWidget::HandleTextCommitted(const FText& Text, const ETextCommit::Type CommitMethod)
{
	// 엔터를 눌렀을 경우만 실행
	if (CommitMethod == ETextCommit::OnEnter)
	{
		const FString InputText = Text.ToString();

		// 입력한 글자 수가 목표 글자 수와 같을 경우만 실행
		if (InputText.Len() == TargetNumberLength)
		{
			// 서버로 입력한 값 전달
			OnInputCommitted.Broadcast(InputText);

			// 입력 창 초기화
			InputTextBox->SetText(FText::GetEmpty());
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UMainWidget::HandleTextChanged(const FText& Text)
{
	FString InputText = Text.ToString();

	// 유효한 입력이 아닐 경우
	if (!IsValidInput(InputText, InputText.Right(1)))
	{
		// 입력 제거(마지막 문자 제거)
		InputText = InputText.LeftChop(1);
	}

	InputTextBox->SetText(Text.FromString(InputText));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UMainWidget::HandleReadyButtonClicked()
{
	if (ANumberBaseballPlayerController* NumberBaseballPlayerController = Cast<ANumberBaseballPlayerController>(
		GetOwningPlayer()))
	{
		NumberBaseballPlayerController->Server_ReadyButtonClicked();

		if (const ANumberBaseballPlayerState* NumberBaseballPlayerState = Cast<ANumberBaseballPlayerState>(
			NumberBaseballPlayerController->PlayerState))
		{
			SetReadyButtonText(NumberBaseballPlayerState->IsReady());
		}
	}
}

bool UMainWidget::IsValidInput(const FString& InputText, const FString& LastChar) const
{
	// 목표 글자 수 보다 커지면 자르기
	// 중복 글자 입력 방지(마지막 글자를 제외한 나머지에서 찾기)
	if (InputText.Len() > TargetNumberLength)
	{
		UE_LOG(LogTemp, Warning, TEXT("글자 수 초과"));
		return false;
	}

	if (InputText.LeftChop(1).Contains(LastChar))
	{
		UE_LOG(LogTemp, Warning, TEXT("중복된 글자"));
		return false;
	}

	return true;
}
