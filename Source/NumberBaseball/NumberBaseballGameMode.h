#pragma once

#include "CoreMinimal.h"
#include "NumberBaseballPlayerState.h"
#include "GameFramework/GameModeBase.h"

#include "NumberBaseballGameMode.generated.h"

UCLASS()
class NUMBERBASEBALL_API ANumberBaseballGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANumberBaseballGameMode();

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	// 플레이어 준비 완료
	void PlayerReady(const ANumberBaseballPlayerState* NumberBaseballPlayerState);
	
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

protected:
	// 랜덤 숫자 길이
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game", meta=(ClampMin = 1, ClampMax = 9))
	int32 TargetNumberLength;
	// 필요 플레이어 수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game", meta=(ClampMin = 1, ClampMax = 4))
	int32 RequiredReadyCount;
	
	// 랜덤 숫자
	FString TargetNumber;
	// 접속한 플레이어 수
	int32 ReadyCount;

	int32 LoginCount;
};
