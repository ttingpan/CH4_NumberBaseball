#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NumberBaseballHUD.generated.h"

class UChatWidget;
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
	TSubclassOf<UChatWidget> ChatWidgetClass;
	
};
