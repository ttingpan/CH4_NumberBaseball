#include "NumberBaseballPlayerState.h"

#include "GameModes/NumberBaseballGameMode.h"
#include "GameModes/NumberBaseballGameState.h"

void ANumberBaseballPlayerState::GameReady() 
{
	if (ANumberBaseballGameMode* NumberBaseballGameMode = Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode()))
	{
		bIsReady = !bIsReady;
		NumberBaseballGameMode->PlayerReady(bIsReady);

		// 준비 버튼 텍스트 변경
		Cast<ANumberBaseballPlayerController>(GetPlayerController())->Client_SetReadyButtonText(bIsReady);

		// 방장 게임시작 버튼 사용가능 설정 업데이트
		if (ANumberBaseballGameState* NumberBaseballGameState
			= Cast<ANumberBaseballGameState>(GetWorld()->GetGameState()))
		{
			NumberBaseballGameState->UpdateHostGameStartButtonIsEnabled();
		}
	}
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
