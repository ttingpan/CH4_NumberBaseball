#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NumberBaseballPlayerState.generated.h"

class ANumberBaseballPlayerController;

UCLASS()
class NUMBERBASEBALL_API ANumberBaseballPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	// 게임 준비 전환
	void GameReady();
	
	ANumberBaseballPlayerController* GetNumberBaseballPlayerController() const;
	
	// 게임 준비 여부 반환
	FORCEINLINE bool IsReady() const { return bIsReady; }
	
private:
	UPROPERTY()
	bool bIsReady = false;
};
