#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ComparingNumbersLib.generated.h"

enum class EBaseballResult : uint8;

UCLASS()
class NUMBERBASEBALL_API UComparingNumbersLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ComparingNumbersLib")
	static FString ComparingNumbers(const FString& InputNumber, const FString& TargetNumber);
};
