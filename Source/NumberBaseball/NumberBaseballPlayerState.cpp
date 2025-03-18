#include "NumberBaseballPlayerState.h"

#include "NumberBaseballGameMode.h"
#include "NumberBaseballGameState.h"
#include "Player/NumberBaseballPlayerController.h"

void ANumberBaseballPlayerState::GameReady()
{
	if (ANumberBaseballGameMode* NumberBaseballGameMode = Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode()))
	{
		bIsReady = !bIsReady;
		NumberBaseballGameMode->PlayerReady(GetPlayerName(), bIsReady);
	}
}
