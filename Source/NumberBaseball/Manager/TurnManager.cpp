#include "TurnManager.h"

ATurnManager::ATurnManager(): TurnDuration(0), RemainingTime(0), MaxTurnCount(0), CurrentTurnCount(0)
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);
}

void ATurnManager::InitTurnManager(const float InTurnDuration, const int32 InMaxTurnCount)
{
	TurnDuration = InTurnDuration;
	RemainingTime = TurnDuration;
	MaxTurnCount = InMaxTurnCount;
	CurrentTurnCount = 0;
}

void ATurnManager::StartTurn()
{
	RemainingTime = TurnDuration;

	OnTurnUpdated.Broadcast(RemainingTime);
	
	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ATurnManager::UpdateTurn, 1.0f, true);
}

void ATurnManager::EndTurn()
{
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	OnTurnEnded.Broadcast();
}

void ATurnManager::UpdateTurn()
{
	RemainingTime -= 1.0f;
	if (RemainingTime <= 0.0f)
	{
		EndTurn();
	}
	else
	{
		OnTurnUpdated.Broadcast(RemainingTime);
	}
}

