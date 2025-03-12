#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "BaseBallGamePlayerController.generated.h"

UCLASS()
class BASEBALLGAME_API ABaseBallGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// 서버에서 브로드캐스팅 받았을 때 동작
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void GotBroadcastMessage(const FString& Message) const;
	
private:
	// 위젯 델리게이트 바인딩
	void BindToWidgetDelegate();
	
	UFUNCTION(Server, Reliable)
	void SetMessageToUserController(const FString& Message);
	// 아이디로 로그인
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void OnLoginWithID(const FString& InUserID);
	
	FString UserID;
};
