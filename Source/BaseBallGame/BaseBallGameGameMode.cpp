#include "BaseBallGameGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Player/BaseBallGamePlayerController.h"

ABaseBallGameGameMode::ABaseBallGameGameMode()
{
	PlayerControllerClass = ABaseBallGamePlayerController::StaticClass();
}

void ABaseBallGameGameMode::GotMessageFromClient(const FString& Message) const
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseBallGamePlayerController::StaticClass(), Actors);

	for (AActor*& Actor: Actors)
	{
		if (const ABaseBallGamePlayerController* Controller = Cast<ABaseBallGamePlayerController>(Actor))
		{
			Controller->GotBroadcastMessage(Message);
		}
	}
}
