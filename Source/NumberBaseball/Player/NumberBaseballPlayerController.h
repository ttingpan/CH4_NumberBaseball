#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "NumberBaseballPlayerController.generated.h"

class ATurnManager;
class UMainWidget;

UCLASS()
class NUMBERBASEBALL_API ANumberBaseballPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// 화면 크기 설정
	static void ChangeGameResolution();
	
	// 턴 업데이트
	UFUNCTION(Client, Unreliable)
	void Client_OnTurnUpdated(float RemainingTime);
	// 턴 종료
	UFUNCTION()
	void OnTurnEnded();
	
	// 상대 이름  설정
	UFUNCTION(Client, Unreliable)
	void Client_SetOtherPlayerName(const FString& OtherPlayerName) const;
	// 게임 참가
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_JoinGame(const FString& PlayerName);
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

	// 위젯 설정
	void SetMainWidget(UMainWidget* InMainWidget);

private:
	UPROPERTY()
	UMainWidget* MainWidget;
};
