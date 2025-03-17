#include "NumberBaseballGameState.h"

#include "GameFramework/PlayerState.h"
#include "Player/NumberBaseballPlayerController.h"

void ANumberBaseballGameState::RegisterPlayerName(const FString& PlayerName)
{
	PlayerNames.Add(PlayerName);
	UE_LOG(LogTemp, Warning, TEXT("RegisterPlayerName: %s"), *PlayerName);
}

void ANumberBaseballGameState::Multicast_UpdateOtherPlayerName_Implementation(const FString& PlayerName1,
                                                                            const FString& PlayerName2)
{
	for (const APlayerState* PlayerState : PlayerArray)
	{
		if (const ANumberBaseballPlayerController* NumberBaseballPlayerController = Cast<ANumberBaseballPlayerController>(
			PlayerState->GetPlayerController()))
		{
			if (PlayerState->GetPlayerName() == PlayerName1)
			{
				NumberBaseballPlayerController->SetOtherPlayerName(PlayerName2);
			}
			else
			{
				NumberBaseballPlayerController->SetOtherPlayerName(PlayerName1);
			}
		}
	}
}
