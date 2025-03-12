#include "BBGGameMode.h"

#include "Player/BBGPlayerController.h"

ABBGGameMode::ABBGGameMode()
{
	PlayerControllerClass = ABBGPlayerController::StaticClass();
}
