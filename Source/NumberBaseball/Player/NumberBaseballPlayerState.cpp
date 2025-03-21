#include "NumberBaseballPlayerState.h"

#include "GameModes/NumberBaseballGameMode.h"
#include "GameModes/NumberBaseballGameState.h"

void ANumberBaseballPlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();
}

void ANumberBaseballPlayerState::GameReady() 
{
	if (ANumberBaseballGameMode* NumberBaseballGameMode = Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode()))
	{
		bIsReady = !bIsReady;
		NumberBaseballGameMode->PlayerReady(bIsReady);

		// 준비 버튼 텍스트 변경
		Cast<ANumberBaseballPlayerController>(GetPlayerController())->Client_SetReadyButtonText(bIsReady);

		// Host(방장) 버튼 설정
		const ANumberBaseballGameState* NumberBaseballGameState
			= NumberBaseballGameMode->GetGameState<ANumberBaseballGameState>();

		if (NumberBaseballGameState->GetIndexByPlayerID(GetUniqueId()) != 0)
		{
			NumberBaseballGameState->GetPlayerControllerByIndex(0)->Client_SetReadyButtonIsEnabled(bIsReady);
		}
	}
}

void ANumberBaseballPlayerState::Server_SetPlayerName_Implementation(const FString& NewPlayerName)
{
	SetPlayerName(NewPlayerName);
	OnRep_PlayerName();
}

bool ANumberBaseballPlayerState::Server_SetPlayerName_Validate(const FString& NewPlayerName)
{
	return true;
}

ANumberBaseballPlayerController* ANumberBaseballPlayerState::GetNumberBaseballPlayerController() const
{
	if (!GetPlayerController())
	{
		return nullptr;
	}

	if (ANumberBaseballPlayerController* NumberBaseballPlayerController
		= Cast<ANumberBaseballPlayerController>(GetPlayerController()))
	{
		return NumberBaseballPlayerController;
	}

	return nullptr;
}
