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

private:
	UPROPERTY(EditDefaultsOnly, category = "UI")
	TSubclassOf<UJoinGameWidget> JoinGameWidgetClass;
	UPROPERTY()
	UJoinGameWidget* JoinGameWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMainWidget> MainWidgetClass;
};
