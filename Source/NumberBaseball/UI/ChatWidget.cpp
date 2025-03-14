#include "ChatWidget.h"

#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void UChatWidget::SetOtherUserName(const FString& OtherUserID) const
{
	if (OtherUserName)
	{
		OtherUserName->SetText(FText::FromString(OtherUserID));
	}
}

void UChatWidget::InitWidget(const int32 InTargetNumberLength)
{
	// 커밋 이벤트 바인딩
	InputTextBox->OnTextCommitted.AddDynamic(this, &UChatWidget::HandleTextCommitted);
	// 텍스트 변경 이벤트 바인딩
	InputTextBox->OnTextChanged.AddDynamic(this, &UChatWidget::HandleTextChanged);

	// 목표 글자 수 설정
	TargetNumberLength = InTargetNumberLength;
	// 도움말 메세지 설정
	const FString HelpMessageText = FString::Printf(TEXT("숫자 %d개를 입력하세요."), TargetNumberLength);
	HelpMessage->SetText(FText::FromString(HelpMessageText));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UChatWidget::HandleTextCommitted(const FText& Text, const ETextCommit::Type CommitMethod)
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
void UChatWidget::HandleTextChanged(const FText& Text)
{
	FString InputText = Text.ToString();
	// 마지막 글자
	const FString LastChar = InputText.Right(1);

	// 목표 글자 수 보다 커지면 자르기
	// 중복 글자 입력 방지(마지막 글자를 제외한 나머지에서 찾기)
	if (InputText.Len() > TargetNumberLength || InputText.LeftChop(1).Contains(LastChar))
	{
		InputText = InputText.LeftChop(1);
	}

	InputTextBox->SetText(Text.FromString(InputText));
}
