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
	
	// 게임 참가
	void JoinGame(const ANumberBaseballPlayerController* PlayerController, const FString& PlayerName);
	// 플레이어 준비 완료
	void PlayerReady(const FString& PlayerName, bool bIsReady);
	// 게임 시작
	void StartGame();
	
	// 클라이언트로부터 메세지를 받았을 때 동작
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_GotInputText(const FString& UserID, const FString& InputText) const;
	
	// 랜덤 숫자 길이 반환
	FORCEINLINE int32 GetTargetNumberLength() const
	{
		return TargetNumberLength;
	}

private:
	// 턴 매니저 클래스
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATurnManager> TurnManagerClass;
	// 턴 매니저
	UPROPERTY()
	ATurnManager* TurnManager;
	// 턴 지속 시간
	UPROPERTY(EditDefaultsOnly, Category = "Game|Turn", meta=(ClampMin = 15.0, ClampMax = 45.0))
	float TurnDuration;
	// 최대 턴 수
	UPROPERTY(EditDefaultsOnly, Category = "Game|Turn", meta=(ClampMin = 3, ClampMax = 5))
	int32 MaxTurnCount;
	// 게임 라운드
	UPROPERTY(EditDefaultsOnly, Category = "Game", meta=(ClampMin = 3, ClampMax = 5))
	int32 MaxRound;
	
	// 랜덤 숫자 길이
	UPROPERTY(EditDefaultsOnly, Category = "Game", meta=(ClampMin = 3, ClampMax = 9))
	int32 TargetNumberLength;
	// 필요 플레이어 수
	UPROPERTY(EditDefaultsOnly, Category = "Game", meta=(ClampMin = 2, ClampMax = 4))
	int32 RequiredReadyCount;
	
	// 랜덤 숫자
	FString TargetNumber;
	// 게임에 참여한 플레이어 수
	int32 JoinedPlayerCount;
	// 준비한 플레이어 수
	int32 ReadyCount;
};
