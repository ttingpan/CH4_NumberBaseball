#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "BaseBallGameGameMode.generated.h"

UCLASS()
class BASEBALLGAME_API ABaseBallGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABaseBallGameGameMode();

	// 클라이언트로부터 메세지를 받았을 때 동작
	void GotMessageFromClient(const FString& Message) const;
};
