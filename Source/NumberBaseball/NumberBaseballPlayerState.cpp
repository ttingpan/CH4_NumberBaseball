#include "NumberBaseballPlayerState.h"

#include "NumberBaseballGameMode.h"
#include "NumberBaseballGameState.h"

void ANumberBaseballPlayerState::GameReady()
{
	if (ANumberBaseballGameMode* NumberBaseballGameMode = Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode()))
	{
		bIsReady = !bIsReady;
		NumberBaseballGameMode->PlayerReady(GetPlayerName(), bIsReady);

		// 준비 버튼 텍스트 변경
		Cast<ANumberBaseballPlayerController>(GetPlayerController())->Client_SetReadyButtonText(bIsReady);

		// Host(방장) 버튼 설정
		ANumberBaseballGameState* NumberBaseballGameState
			= NumberBaseballGameMode->GetGameState<ANumberBaseballGameState>();

		if (NumberBaseballGameState->GetPlayerControllerByIndex(0) != GetPlayerController())
		{
			NumberBaseballGameState->GetPlayerControllerByIndex(0)->Client_SetReadyButtonIsEnabled(bIsReady);
		}
	}
}
