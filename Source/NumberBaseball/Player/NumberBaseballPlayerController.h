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
	
	// 턴 시작
	UFUNCTION(Client, Reliable)
	void Client_OnTurnStarted();
	// 턴 업데이트
	UFUNCTION(Client, Unreliable)
	void Client_OnTurnUpdated(float RemainingTime);
	// 턴 종료
	UFUNCTION(Client, Reliable)
	void Client_OnTurnEnded(bool bIsAuto);

	// 플레이어 이름 등록
	void SetPlayerName(const FString& NewPlayerName) const;
	UFUNCTION(server, Reliable, WithValidation)
	void Server_SetPlayerName(const FString& NewPlayerName);
	// 게임 참가 시도
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_JoinGame();
	// 게임 참가
	UFUNCTION(Client, Reliable)
	void Client_JoinGame(int32 PlayerIndex);
	// 다른 플레이어 참가
	UFUNCTION(Client, Reliable)
	void Client_UpdatePlayerSlotWidget();
	// 준비 버튼 클릭
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ReadyButtonClicked();
	// 준비 버튼 텍스트 설정
	UFUNCTION(Client, Unreliable)
	void Client_SetReadyButtonText(bool bIsReady);
	// 게임 시작 버튼 활성화
	UFUNCTION(Client, Reliable)
	void Client_SetReadyButtonIsEnabled(bool bIsReady);
	
	// 게임 시작
	UFUNCTION(Client, Reliable)
	void Client_PrepareGameStart(const int32& TargetNumberLength);
	// 서버로 입력 내용 전달
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetInputText(const FString& InputText);

	// 채팅 위젯 추가
	UFUNCTION(Client, Reliable)
	void Client_AddChatWidget(int32 JoinedIndex, const FString& InputText);
	// 판정 결과 갱신
	UFUNCTION(Client, Reliable)
	void Client_UpdateResult(int32 StrikeCount, int32 BallCount);

	// 위젯 설정
	void SetMainWidget(UMainWidget* InMainWidget);

private:
	UPROPERTY()
	TObjectPtr<UMainWidget> MainWidget;
};
