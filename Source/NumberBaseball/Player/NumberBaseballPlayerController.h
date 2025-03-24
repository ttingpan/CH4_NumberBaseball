#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "NumberBaseballPlayerController.generated.h"

class ANumberBaseballPlayerState;
class UJoinGameWidget;
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
	// 플레이어 위젯 업데이트
	UFUNCTION(Client, Reliable)
	void Client_UpdatePlayerSlotWidget() const;
	
	// 게임 참가 시도
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_JoinGame(const FString& NewPlayerName);
	// 게임 참가
	UFUNCTION(Client, Reliable)
	void Client_JoinGame(bool bIsHost);
	// 다른 플레이어 위젯 업데이트
	UFUNCTION(Client, Reliable)
	void Client_UpdateOtherPlayerName(int32 Index, const FString& OtherPlayerName) const;
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
	void Client_AddChatWidget(const ANumberBaseballPlayerState* ChatOwnerPlayerState, const FString& InputText);
	// 판정 결과 갱신
	UFUNCTION(Client, Reliable)
	void Client_UpdateResult(int32 StrikeCount, int32 BallCount);

	// 점수 갱신
	UFUNCTION(Client, Reliable)
	void Client_UpdateScore(ANumberBaseballPlayerState* WinnerPlayerState, int32 Score);

	// 채팅 라운드 알림 위젯 추가
	UFUNCTION(Client, Reliable)
	void Client_AddChatRoundNotifyWidget(const int32 CurrentRound, bool bIsStart);
	// 라운드 텍스트 업데이트
	UFUNCTION(Client, Reliable)
	void Client_UpdateRoundText(const int32 CurrentRound);
	// 턴 텍스트 업데이트
	UFUNCTION(Client, Reliable)
	void Client_UpdateTurnText(const int32 CurrentTurn);

	// 메인 위젯 설정
	void SetMainWidget(UMainWidget* InMainWidget);

	// 게임 참가 버튼 활성화
	void SetJoinButtonIsEnabled() const;

	// 게임 참가 위젯 설정
	FORCEINLINE void SetJoinGameWidget(UJoinGameWidget* InJoinGameWidget)
	{
		JoinGameWidget = InJoinGameWidget;
	}

private:
	UPROPERTY()
	TObjectPtr<UJoinGameWidget> JoinGameWidget;

	UPROPERTY()
	TObjectPtr<UMainWidget> MainWidget;
};
