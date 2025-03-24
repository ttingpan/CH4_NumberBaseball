#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NumberBaseballHUD.generated.h"

class UJoinGameWidget;
class UMainWidget;

UCLASS()
class NUMBERBASEBALL_API ANumberBaseballHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void JoinGame() const;

	void AddChatWidget(const FString& PlayerName, const FString& InputText) const;

	void AddChatRoundNotifyWidget(const int32 CurrentRound, const bool bIsStart) const;

private:
	// 참가 위젯
	UPROPERTY(EditDefaultsOnly, category = "UI")
	TSubclassOf<UJoinGameWidget> JoinGameWidgetClass;
	UPROPERTY()
	UJoinGameWidget* JoinGameWidget;

	// 메인 위젯
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMainWidget> MainWidgetClass;
	UPROPERTY()
	UMainWidget* MainWidget;

	// 채팅 위젯
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UChatWidget> ChatWidgetClass;

	// 플레이어 슬롯 위젯
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UPlayerSlotWidget> PlayerSlotWidgetClass;

	// 채팅 라운드 알림 위젯
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UChatRoundNotifyWidget> ChatRoundNotifyWidgetClass;
};
