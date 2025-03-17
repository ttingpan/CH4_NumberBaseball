#include "NumberBaseballPlayerState.h"

#include "NumberBaseballGameMode.h"
#include "NumberBaseballGameState.h"

ANumberBaseballPlayerState::ANumberBaseballPlayerState(): bIsReady(false)
{
}

void ANumberBaseballPlayerState::GameReady()
{
	bIsReady = !bIsReady;
	
	if (ANumberBaseballGameMode* NumberBaseballGameMode = Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode()))
	{
		NumberBaseballGameMode->PlayerReady(this);
	}
}

