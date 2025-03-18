#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InputValidationLib.generated.h"

UCLASS()
class NUMBERBASEBALL_API UInputValidationLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 입력한 텍스트가 유효한지 검사
	static bool IsValidInputText(
		const FString& InputText,
		const int32 MaxLength,
		const FRegexPattern& Pattern,
		bool bIsDuplicable = false);
};
