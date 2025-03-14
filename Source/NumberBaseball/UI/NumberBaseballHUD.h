#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NumberBaseballHUD.generated.h"

class UChatWidget;

UCLASS()
class NUMBERBASEBALL_API ANumberBaseballHUD : public AHUD
{
	GENERATED_BODY()

public:
	void Init();

	// 채팅 위젯 반환
	FORCEINLINE UChatWidget* GetChatWidget() const { return ChatWidget; }

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UChatWidget> ChatWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<UChatWidget> ChatWidget;
};
