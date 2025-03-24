#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player/NumberBaseballPlayerController.h"

#include "NumberBaseballGameMode.generated.h"

class ATurnManager;

UCLASS()
class NUMBERBASEBALL_API ANumberBaseballGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANumberBaseballGameMode();

	virtual void BeginPlay() override;
	
	// 플레이어 준비 완료
	void PlayerReady(bool bIsReady);
	// 게임 시작
	void StartGame();

	// 클라이언트로부터 메세지를 받았을 때 동작
	void GotInputText(ANumberBaseballPlayerState* ChatOwnerPlayerState, const FString& InputText);

	// 시간 초과로 턴이 종료 되었을 때
	void TurnTimeOver(const ANumberBaseballPlayerState* ChatOwnerPlayerState) const;

	// 클라이언트에게 결과 전달
	void SendInputText(int32 StrikeCount, int32 BallCount, ANumberBaseballPlayerState* ChatOwnerPlayerState);

	// 랜덤 숫자 길이 반환
	FORCEINLINE int32 GetTargetNumberLength() const
	{
		return TargetNumberLength;
	}

	// 게임 최대 라운드 반환
	FORCEINLINE int32 GetMaxGameRound() const { return MaxGameRound; }

	// 필요 준비 카운트
	FORCEINLINE void SetRequiredReadyCount(const int32 InRequiredReadyCount) { RequiredReadyCount = InRequiredReadyCount; }

private:
	// 턴 매니저 클래스
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATurnManager> TurnManagerClass;
	// 턴 매니저
	UPROPERTY()
	ATurnManager* TurnManager;
	// 턴 지속 시간
	UPROPERTY(EditDefaultsOnly, Category = "Game|Turn", meta=(ClampMin = 5.0, ClampMax = 45.0))
	float TurnDuration;
	// 플레이어 턴 수(총 턴 수 = 플레이어 턴 수 * 총 플레이어 수)
	UPROPERTY(EditDefaultsOnly, Category = "Game|Turn", meta=(ClampMin = 2, ClampMax = 5))
	int32 MaxTurnCount;
	// 게임 최대 라운드
	UPROPERTY(EditDefaultsOnly, Category = "Game", meta=(ClampMin = 3, ClampMax = 5))
	int32 MaxGameRound;

	// 랜덤 숫자 길이
	UPROPERTY(EditDefaultsOnly, Category = "Game", meta=(ClampMin = 3, ClampMax = 9))
	int32 TargetNumberLength;
	// 필요 플레이어 수
	int32 RequiredReadyCount;

	// 랜덤 숫자
	FString TargetNumber;
	// 준비한 플레이어 수
	int32 ReadyCount;
};
