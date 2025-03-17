#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "NumberBaseballPlayerController.generated.h"

class UMainWidget;

UCLASS()
class NUMBERBASEBALL_API ANumberBaseballPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// 화면 크기 설정
	static void ChangeGameResolution();

	void SetOtherPlayerName(const FString& OtherPlayerName) const;
	
	// 준비 버튼 클릭
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ReadyButtonClicked();
	
	// 게임 시작
	UFUNCTION(Client, Reliable)
	void GameStarted(const int32& TargetNumberLength);
	
	// 서버에서 브로드캐스팅 받았을 때 동작
	UFUNCTION(Client, Reliable)
	void Client_GotInputText(const FString& Message) const;
	// 입력 내용 전달
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetInputText(const FString& InputText);

	FORCEINLINE void SetMainWidget(UMainWidget* InMainWidget) { MainWidget = InMainWidget; }

private:
	UPROPERTY()
	UMainWidget* MainWidget;
};
