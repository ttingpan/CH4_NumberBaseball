#include "JoinGameWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "FunctionLibrary/InputValidationLib.h"
#include "GameFramework/PlayerState.h"
#include "Player/NumberBaseballPlayerController.h"

void UJoinGameWidget::InitWidget()
{
	PlayerNameTextBox->OnTextChanged.AddDynamic(this, &UJoinGameWidget::OnTextChanged);
	JoinButton->OnClicked.AddDynamic(this, &UJoinGameWidget::OnJoinButtonClicked);
}

void UJoinGameWidget::SetJoinButtonIsEnabled() const
{
	JoinButton->SetIsEnabled(true);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UJoinGameWidget::OnTextChanged(const FText& Text)
{
	FString InputText = Text.ToString();

	// 입력 검증 => 알파벳, 숫자만 입력 가능, 중복 가능
	if (!UInputValidationLib::IsValidInputText(InputText, 10, FRegexPattern(TEXT("^[a-zA-Z0-9]*$")), true))
	{
		// 입력 제거(마지막 문자 제거)
		InputText = InputText.LeftChop(1);
	}

	PlayerNameTextBox->SetText(Text.FromString(InputText));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UJoinGameWidget::OnJoinButtonClicked()
{
	if (ANumberBaseballPlayerController* NumberBaseballPlayerController
		= Cast<ANumberBaseballPlayerController>(GetOwningPlayer()))
	{
		FString NewPlayerName = PlayerNameTextBox->GetText().ToString();
		if (NewPlayerName.IsEmpty())
		{
			NewPlayerName = TEXT("Unknown");
		}
		NumberBaseballPlayerController->PlayerState->SetPlayerName(NewPlayerName);
		NumberBaseballPlayerController->Server_JoinGame(NewPlayerName);
	}
}
