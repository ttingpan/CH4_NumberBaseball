#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NumberBaseballGameState.generated.h"

class ANumberBaseballPlayerController;

UCLASS()
class NUMBERBASEBALL_API ANumberBaseballGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ANumberBaseballGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void RegisterUserID(const FString& UserID);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateUserIDs();
	
	// 목표 숫자 길이 반환
	FORCEINLINE int32 GetTargetNumberLength() const { return TargetNumberLength; }
	// 목표 숫자 길이 설정
	FORCEINLINE void SetTargetNumberLength(const int32 NewTargetNumberLength) { TargetNumberLength = NewTargetNumberLength; }
private:
	// 서버에서 생성한 목표 숫자 길이
	UPROPERTY(Replicated)
	int32 TargetNumberLength;

	// 전체 플레이어의 아이디
	TArray<FString> UserIDs;
};
