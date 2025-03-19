#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NumberBaseballGameState.generated.h"

class ANumberBaseballPlayerController;
class ATurnManager;

UCLASS()
class NUMBERBASEBALL_API ANumberBaseballGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 플레이어 등록
	void RegisterPlayer(ANumberBaseballPlayerController* PlayerController, const FString& PlayerName);

	// 상대 이름 등록
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_UpdateOtherPlayerName();

	// 턴 매니저 이벤트 바인딩
	void BindTurnManagerEvent();

	// 턴 시작
	UFUNCTION()
	void OnTurnStarted();
	// 턴 업데이트
	UFUNCTION()
	void OnTurnUpdated(float RemainingTime);
	// 턴 종료
	UFUNCTION()
	void OnTurnEnded();
	// 턴 즉시 종료
	UFUNCTION()
	void OnTurnEndedImmediately();
	// 다음 턴 시작
	UFUNCTION()
	void StartNextRound() const;

	FORCEINLINE ANumberBaseballPlayerController* GetPlayerControllerByIndex(const int32 Index)
	{
		return PlayerControllers[Index];
	}

	FORCEINLINE ATurnManager* GetTurnManager() const { return TurnManager; }
	FORCEINLINE void SetTurnManager(ATurnManager* InTurnManager) { TurnManager = InTurnManager; }

private:
	
	// 게임에 참여한 플레이어 이름 목록
	UPROPERTY(Replicated)
	TArray<FString> PlayerNames;

	UPROPERTY(Replicated)
	TArray<ANumberBaseballPlayerController*> PlayerControllers;

	// 현재 턴 플레이어 인덱스
	UPROPERTY(Replicated)
	int32 CurrentTurnPlayerIndex = 0;
	// 턴 매니저
	UPROPERTY(Replicated)
	ATurnManager* TurnManager;
	// 현재 게임 라운드
	UPROPERTY(Replicated)
	int32 CurrentRound = 1;
};
