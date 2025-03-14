#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "NumberBaseballPlayerController.generated.h"

class UChatWidget;

UCLASS()
class NUMBERBASEBALL_API ANumberBaseballPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	// 서버에서 브로드캐스팅 받았을 때 동작
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_GotInputText(const FString& Message) const;

	// 플레이어 로그인
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LogInPlayer();

	// 위젯 초기화
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_InitWidget();

	// 입력 내용 전달
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetInputText(const FString& InputText);

	UFUNCTION(Server, Reliable)
	void Server_RegisterUserID(const FString& UserID);

	UFUNCTION(Server, Reliable)
	void Server_NotifyAllLogin();

	UFUNCTION(Client, Reliable)
	void Client_UpdateOtherUserName(const FString& OtherUserID);

private:
	// 채팅 위젯
	UPROPERTY()
	UChatWidget* ChatWidget;
};
