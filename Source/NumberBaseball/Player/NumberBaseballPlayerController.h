#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "NumberBaseballPlayerController.generated.h"

UCLASS()
class NUMBERBASEBALL_API ANumberBaseballPlayerController : public APlayerController
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

	// 입력 내용 전달
	UFUNCTION(Server, Reliable)
	void SetMessageToUserController(const FString& InputText);
	// 아이디로 로그인
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void OnLoginWithID(const FString& InUserID);
	
	FString UserID;
};
