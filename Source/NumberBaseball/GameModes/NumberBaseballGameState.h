#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NumberBaseballGameState.generated.h"

class ANumberBaseballPlayerState;
class ANumberBaseballPlayerController;
class ATurnManager;

UCLASS()
class NUMBERBASEBALL_API ANumberBaseballGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 게임 참가
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_JoinGame();
	// 다른 플레이어 참가
	void JoinOtherPlayer();

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

	int32 GetIndexByPlayerID(const FUniqueNetIdRepl& PlayerID) const;

	ANumberBaseballPlayerController* GetPlayerControllerByUniqueID(const FUniqueNetIdRepl& PlayerID) const;
	ANumberBaseballPlayerController* GetPlayerControllerByIndex(const int32 Index) const;

	FORCEINLINE void GetJoinedPlayerIDs(TArray<FUniqueNetIdRepl>& OutJoinedPlayerIDs) const { OutJoinedPlayerIDs = JoinedPlayerIDs; }
	FORCEINLINE ATurnManager* GetTurnManager() const { return TurnManager; }
	FORCEINLINE void SetTurnManager(ATurnManager* InTurnManager) { TurnManager = InTurnManager; }

private:
	UFUNCTION()
	void OnRep_JoinedPlayerIDs();

	// 게임에 참여한 플레이어 아이디 목록
	UPROPERTY(ReplicatedUsing=OnRep_JoinedPlayerIDs)
	TArray<FUniqueNetIdRepl> JoinedPlayerIDs;

	// 현재 턴 플레이어 인덱스
	UPROPERTY(Replicated)
	int32 CurrentTurnPlayerIndex = 0;
	// 턴 매니저
	UPROPERTY(Replicated)
	TObjectPtr<ATurnManager> TurnManager;
	// 현재 게임 라운드
	UPROPERTY(Replicated)
	int32 CurrentRound = 1;
};
