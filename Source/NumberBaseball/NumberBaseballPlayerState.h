#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NumberBaseballPlayerState.generated.h"

UCLASS()
class NUMBERBASEBALL_API ANumberBaseballPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ANumberBaseballPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// 유저 아이디 반환
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString GetUserID() const { return UserID; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsLoggedIn() const { return bIsLoggedIn; }
	
	// 유저 아이디 설정
	FORCEINLINE void LogIn(const FString& InUserID)
	{
		UserID = InUserID;
		bIsLoggedIn = true;
	}

	
private:
	UPROPERTY(Replicated)
	FString UserID;

	// 로그인 여부
	UPROPERTY(Replicated)
	bool bIsLoggedIn = false;
};
