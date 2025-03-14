#include "NumberBaseballPlayerState.h"

#include "NumberBaseballGameState.h"
#include "Net/UnrealNetwork.h"

ANumberBaseballPlayerState::ANumberBaseballPlayerState()
{
	UserID = "Unknown";
}

void ANumberBaseballPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANumberBaseballPlayerState, UserID);
	DOREPLIFETIME(ANumberBaseballPlayerState, bIsLoggedIn);
}

