#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurnManager.generated.h"

// 남은 시간 업데이트 이벤트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTurnUpdated, float, RemainingTime);
// 턴 종료 이벤트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTurnEnded);

UCLASS()
class NUMBERBASEBALL_API ATurnManager : public AActor
{
	GENERATED_BODY()

public:
	ATurnManager();
	
	// 타이머 매니저 초기화
	void InitTurnManager(float InTurnDuration, int32 InMaxTurnCount);

	// 턴 시작
	void StartTurn();
	// 턴 종료
	void EndTurn();

	// 턴 지속 시간 반환
	FORCEINLINE float GetTurnDuration() const { return TurnDuration; }
	// 턴 남은 시간 반환
	FORCEINLINE float GetRemainingTime() const { return RemainingTime; }

	// 남은 시간 업데이트 이벤트
	FTurnUpdated OnTurnUpdated;
	// 턴 종료 이벤트
	FTurnEnded OnTurnEnded;
	
private:
	// 타이머 갱신
	void UpdateTurn();
	
	// 턴 지속 시간 (초)
	float TurnDuration;
	// 현재 남은 시간 (초)
	float RemainingTime;
	
	// 타이머 핸들
	FTimerHandle TurnTimerHandle;

	// 최대 턴 횟수 => 플레이어 1 입력, 플레이어 2 입력 = 1턴
	int32 MaxTurnCount;
	// 현재 턴 수
	int32 CurrentTurnCount;
};
