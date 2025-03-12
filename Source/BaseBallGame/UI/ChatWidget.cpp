#include "ChatWidget.h"

#include "Components/EditableTextBox.h"

void UChatWidget::InitWidget()
{
	// 커밋 이벤트 바인딩
	ChatTextBox->OnTextCommitted.AddDynamic(this, &UChatWidget::HandleTextCommitted);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UChatWidget::HandleTextCommitted(const FText& Text, const ETextCommit::Type CommitMethod)
{
	// 엔터를 눌렀을 경우만 실행
	if (CommitMethod == ETextCommit::OnEnter)
	{
		const FString Message = Text.ToString();

		// 커밋 델리게이트 실행
		OnMessageCommitted.Broadcast(Message);

		// 입력 창 초기화
		ChatTextBox->SetText(FText::GetEmpty());
	}
}
