#include "InputValidationLib.h"

bool UInputValidationLib::IsValidInputText(
	const FString& InputText,
	const int32 MaxLength,
	const FRegexPattern& Pattern,
	const bool bIsDuplicable)
{
	// 정규식에 맞는지 검사
	if (!FRegexMatcher(Pattern, InputText).FindNext())
	{
		UE_LOG(LogTemp, Warning, TEXT("입력 조건에 맞지 않습니다."));
		return false;
	}

	// 길이 검사
	if (InputText.Len() > MaxLength)
	{
		UE_LOG(LogTemp, Warning, TEXT("글자 수 초과"));
		return false;
	}

	// 중복 검사
	if (!bIsDuplicable && InputText.LeftChop(1).Contains(InputText.Right(1)))
	{
		UE_LOG(LogTemp, Warning, TEXT("중복된 글자"));
		return false;
	}

	return true;
}
