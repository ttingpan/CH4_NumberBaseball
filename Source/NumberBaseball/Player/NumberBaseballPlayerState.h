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
	
	virtual void OnRep_PlayerName() override;
	
	// 게임 준비 전환
	void GameReady();

	// 플레이어 이름 설정
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetPlayerName(const FString& NewPlayerName);
	
	ANumberBaseballPlayerController* GetNumberBaseballPlayerController() const;
	
	// 게임 준비 여부 반환
	FORCEINLINE bool IsReady() const { return bIsReady; }
	
private:
	UPROPERTY()
	bool bIsReady = false;
};
