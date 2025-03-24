#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurnManager.generated.h"

// 턴 시작 이벤트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTurnStarted);

// 남은 시간 업데이트 이벤트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTurnUpdated, float, RemainingTime);

// 턴 종료 이벤트 -> 자동
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTurnEnded);

// 턴 즉시 종료 이벤트 -> 플레이어 입력시
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTurnEndedImmediately);

UCLASS()
class NUMBERBASEBALL_API ATurnManager : public AActor
{
	GENERATED_BODY()

public:
	ATurnManager();

	// 타이머 매니저 초기화
	void InitTurnManager(float InTurnDuration, int32 InMaxTurnCount);
	// 턴 시작 준비
	void PrepareTurnStart();
	// 턴 시작
	void StartTurn();
	// 턴 종료
	void EndTurn(bool bIsAuto = true);

	// 턴 지속 시간 반환
	FORCEINLINE float GetTurnDuration() const { return TurnDuration; }
	// 턴 남은 시간 반환
	FORCEINLINE float GetRemainingTime() const { return RemainingTime; }
	// 현재 턴 반환
	FORCEINLINE int32 GetCurrentTurnCount() const { return CurrentTurnCount; }
	// 현재 최대 턴인지 확인
	FORCEINLINE bool IsMaxTurn() const { return CurrentTurnCount == MaxTurnCount; }

	// 현재 턴 설정
	FORCEINLINE void SetCurrentTurnCount(const int32 InCurrentTurnCount)
	{
		CurrentTurnCount = InCurrentTurnCount;
	}

	// 턴 시작 이벤트
	FTurnStarted OnTurnStarted;
	// 남은 시간 업데이트 이벤트
	FTurnUpdated OnTurnUpdated;
	// 턴 종료 이벤트
	FTurnEnded OnTurnEnded;
	// 턴 즉시 종료 이벤트
	FTurnEndedImmediately OnTurnEndedImmediately;

private:
	// 타이머 갱신
	void UpdateTurn();

	// 턴 지속 시간 (초)
	float TurnDuration;
	// 현재 남은 시간 (초)
	float RemainingTime;

	// 타이머 핸들
	FTimerHandle TurnTimerHandle;

	// 최대 턴 횟수
	int32 MaxTurnCount;
	// 현재 턴 수
	int32 CurrentTurnCount;
};
