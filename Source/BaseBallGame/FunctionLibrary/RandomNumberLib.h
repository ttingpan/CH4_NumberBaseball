#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RandomNumberLib.generated.h"

UCLASS()
class BASEBALLGAME_API URandomNumberLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RandomNumberLib")
	static FString GenerateRandomNumber(int32 Length);
};
