#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NumberBaseballHUD.generated.h"

class UMainWidget;

UCLASS()
class NUMBERBASEBALL_API ANumberBaseballHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMainWidget> MainWidgetClass;
};
