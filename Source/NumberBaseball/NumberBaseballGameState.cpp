#include "NumberBaseballGameState.h"

#include "GameFramework/PlayerState.h"
#include "Manager/TurnManager.h"
#include "Net/UnrealNetwork.h"
#include "Player/NumberBaseballPlayerController.h"

void ANumberBaseballGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANumberBaseballGameState, PlayerNames);
	DOREPLIFETIME(ANumberBaseballGameState, CurrentTurnPlayerIndex);
	DOREPLIFETIME(ANumberBaseballGameState, TurnManager);
}

void ANumberBaseballGameState::RegisterPlayerName(const FString& PlayerName)
{
	PlayerNames.Add(PlayerName);
	UE_LOG(LogTemp, Warning, TEXT("RegisterPlayerName: %s"), *PlayerName);

	// 상대 이름 설정
	Multicast_UpdateOtherPlayerName();
}

void ANumberBaseballGameState::BindTurnManagerEvent()
{
	if (TurnManager)
	{
		TurnManager->OnTurnUpdated.AddDynamic(this, &ANumberBaseballGameState::OnTurnUpdated);
	}
}

void ANumberBaseballGameState::OnTurnUpdated(const float RemainingTime)
{
	// 모든 플레이어 컨트롤러를 순회하며 UI 업데이트
	for (const APlayerState* PlayerState : PlayerArray)
	{
		if (PlayerState)
		{
			if (ANumberBaseballPlayerController* NumberBaseballPlayerController
				= Cast<ANumberBaseballPlayerController>(PlayerState->GetOwner()))
			{
				NumberBaseballPlayerController->Client_OnTurnUpdated(RemainingTime);
			}
		}
	}
}

void ANumberBaseballGameState::Multicast_UpdateOtherPlayerName_Implementation()
{
	if (PlayerNames.Num() <= 1)
	{
		return;
	}

	for (const APlayerState* PlayerState : PlayerArray)
	{
		if (const ANumberBaseballPlayerController* NumberBaseballPlayerController = Cast<
			ANumberBaseballPlayerController>(
			PlayerState->GetPlayerController()))
		{
			if (PlayerState->GetPlayerName() == PlayerNames[0])
			{
				NumberBaseballPlayerController->Client_SetOtherPlayerName(PlayerNames[1]);
			}
			else
			{
				NumberBaseballPlayerController->Client_SetOtherPlayerName(PlayerNames[0]);
			}
		}
	}
}
