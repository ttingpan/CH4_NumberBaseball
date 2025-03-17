#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NumberBaseballGameState.generated.h"

class ANumberBaseballPlayerController;

UCLASS()
class NUMBERBASEBALL_API ANumberBaseballGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	void RegisterPlayerName(const FString& PlayerName);
	
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_UpdateOtherPlayerName(const FString& PlayerName1, const FString& PlayerName2);

private:
	TArray<FString> PlayerNames;
};
