#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "NumberBaseballGameMode.generated.h"

UCLASS()
class NUMBERBASEBALL_API ANumberBaseballGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANumberBaseballGameMode();

	virtual void BeginPlay() override;
	
	// 클라이언트로부터 메세지를 받았을 때 동작
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_GotInputText(const FString& UserID, const FString& InputText) const;
	
	// 랜덤 숫자 길이 반환
	FORCEINLINE int32 GetTargetNumberLength() const
	{
		return TargetNumberLength;
	}
	// 랜덤 숫자 반환
	FORCEINLINE FString GetTargetNumber() const
	{
		return TargetNumber;
	}
	
	// 랜덤 숫자 길이 설정
	FORCEINLINE void SetTargetNumberLength(const int32 InTargetNumberLength)
	{
		TargetNumberLength = InTargetNumberLength;
	}

protected:
	// 랜덤 숫자 길이
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TargetNumber", meta=(ClampMin = 1, ClampMax = 9))
	int32 TargetNumberLength;

	// 랜덤 숫자
	FString TargetNumber;
};
