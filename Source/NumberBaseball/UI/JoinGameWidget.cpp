#include "JoinGameWidget.h"

#include "NumberBaseballHUD.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "FunctionLibrary/InputValidationLib.h"
#include "Player/NumberBaseballPlayerController.h"

void UJoinGameWidget::InitWidget()
{
	PlayerNameTextBox->OnTextChanged.AddDynamic(this, &UJoinGameWidget::OnTextChanged);
	JoinButton->OnClicked.AddDynamic(this, &UJoinGameWidget::OnJoinButtonClicked);
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
	if (const ANumberBaseballHUD* NumberBaseballHUD = Cast<ANumberBaseballHUD>(GetOwningPlayer()->GetHUD()))
	{
		if (ANumberBaseballPlayerController* NumberBaseballPlayerController = Cast<ANumberBaseballPlayerController>(
			GetOwningPlayer()))
		{
			FString PlayerName = PlayerNameTextBox->GetText().ToString();
			if (PlayerName.IsEmpty())
			{
				PlayerName = TEXT("Unknown");				
			}
			NumberBaseballHUD->JoinGame();
			NumberBaseballPlayerController->Server_JoinGame(PlayerName);
		}
	}
}
