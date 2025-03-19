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
	MaxTurnCount = InMaxTurnCount * 2;
	CurrentTurnCount = -1;
}

void ATurnManager::PrepareTurnStart()
{
	const float DefaultTurnDuration = TurnDuration;
	TurnDuration = 3.0f;

	StartTurn();

	TurnDuration = DefaultTurnDuration;
}

void ATurnManager::StartTurn()
{
	RemainingTime = TurnDuration;

	// 턴 증가
	CurrentTurnCount++;

	// 준비 턴에선 시작 x
	if (CurrentTurnCount > 0)
	{
		OnTurnStarted.Broadcast();
	}
	
	OnTurnUpdated.Broadcast(RemainingTime);

	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ATurnManager::UpdateTurn, 1.0f, true);
}

void ATurnManager::EndTurn(const bool bIsAuto)
{
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	
	if (bIsAuto)
	{
		OnTurnEnded.Broadcast();
	}
	else
	{
		OnTurnEndedImmediately.Broadcast();
	}
}

void ATurnManager::UpdateTurn()
{
	RemainingTime -= 1.0f;

	OnTurnUpdated.Broadcast(RemainingTime);

	if (RemainingTime <= 0.0f)
	{
		EndTurn();
	}
}
