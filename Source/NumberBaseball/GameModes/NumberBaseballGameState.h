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
	void JoinGame(ANumberBaseballPlayerState* JoinPlayerState, const FString& NewPlayerName);
	// 다른 플레이어 참가
	void UpdateOtherPlayer();

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
	// 처음 라운드 준비
	void PrepareStartGame(int32 TargetNumberLength);
	// 다음 라운드 준비
	UFUNCTION()
	void PrepareStartNextRound();
	// 다음 라운드 시작
	UFUNCTION()
	void StartNextRound();

	// 채팅 추가
	void AddChatWidget(const int32 Index, const FString& InputText);

	// 준비 완료 표시 설정
	void UpdateVisibilityReadyTextBorder() const;
	void UpdateVisibilityReadyTextBorder(int32 Index, bool bIsReady) const;
	// 플레이어 점수 획득
	void AddPlayerScore(int32 Index, int32 WinScore);

	// 방장 게임시작 버튼 사용가능 업데이트
	void UpdateHostGameStartButtonIsEnabled();
	// 모든 플레이어 라운드 텍스트 업데이트
	void UpdateRoundText();
	// 모든 플레이어 턴 텍스트 업데이트
	void UpdateTurnText();

	// 게임 종료 위젯 표시
	UFUNCTION()
	void ShowGameOverWidget(const APlayerState* WinnerPlayerState);

	FORCEINLINE TArray<ANumberBaseballPlayerState*> GetJoinedPlayerStates() const
	{
		return JoinedPlayerStates;
	}
	FORCEINLINE int32 GetJoinedPlayerCount() const { return JoinedPlayerStates.Num(); }
	
	FORCEINLINE TArray<ANumberBaseballPlayerController*> GetJoinedPlayerControllers() const
	{
		return JoinedPlayerControllers;
	}
	FORCEINLINE int32 GetCurrentTurnPlayerIndex() const { return CurrentTurnPlayerIndex; }

	FORCEINLINE ATurnManager* GetTurnManager() const { return TurnManager; }
	FORCEINLINE void SetTurnManager(ATurnManager* InTurnManager) { TurnManager = InTurnManager; }

private:
	// 게임에 참여한 플레이어 스테이트 목록
	UPROPERTY(Replicated)
	TArray<TObjectPtr<ANumberBaseballPlayerState>> JoinedPlayerStates;
	// 게임에 참여한 플레이어 컨트롤러 목록
	UPROPERTY(replicated)
	TArray<TObjectPtr<ANumberBaseballPlayerController>> JoinedPlayerControllers;
	// 현재 턴 플레이어 인덱스
	UPROPERTY(Replicated)
	int32 CurrentTurnPlayerIndex = 0;
	// 이전 턴 플레이어 컨트롤러
	TObjectPtr<ANumberBaseballPlayerController> BeforeTurnPlayerController;

	// 턴 매니저
	UPROPERTY(Replicated)
	TObjectPtr<ATurnManager> TurnManager;
	// 현재 게임 라운드
	UPROPERTY(Replicated)
	int32 CurrentRound = 1;

	// 게임 시작 여부
	bool GameStarted = false;
};
