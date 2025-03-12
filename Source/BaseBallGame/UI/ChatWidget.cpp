#include "ChatWidget.h"

#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void UChatWidget::InitWidget()
{
	// 커밋 이벤트 바인딩
	InputTextBox->OnTextCommitted.AddDynamic(this, &UChatWidget::HandleTextCommitted);
	// 텍스트 변경 이벤트 바인딩
	InputTextBox->OnTextChanged.AddDynamic(this, &UChatWidget::HandleTextChanged);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UChatWidget::HandleTextCommitted(const FText& Text, const ETextCommit::Type CommitMethod)
{
	// 엔터를 눌렀을 경우만 실행
	if (CommitMethod == ETextCommit::OnEnter)
	{
		const FString InputText = Text.ToString();

		// 커밋 델리게이트 실행
		OnInputCommitted.Broadcast(InputText);

		// 입력 창 초기화
		InputTextBox->SetText(FText::GetEmpty());
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UChatWidget::HandleTextChanged(const FText& Text)
{
	// 최대 9글자까지 입력 가능
	FString InputText = Text.ToString();
	FString ErrorMessage = "";
	if (InputText.Len() > 9)
	{
		ErrorMessage = TEXT("최대 9글자까지 입력 가능합니다.");
		InputText.RemoveAt(9);
		InputTextBox->SetText(Text.FromString(InputText));
	}

	ErrorMessageText->SetText(FText::FromString(ErrorMessage));
}
