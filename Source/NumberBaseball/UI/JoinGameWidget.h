#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "JoinGameWidget.generated.h"

UCLASS(Abstract)
class NUMBERBASEBALL_API UJoinGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 위젯 초기화
	void InitWidget();

	// 참가 버튼 활성화
	void SetJoinButtonIsEnabled() const;

private:
	UFUNCTION()
	void OnTextChanged(const FText& Text);
	UFUNCTION()
	void OnJoinButtonClicked();

	// 플레이어 입력 텍스트 박스
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* PlayerNameTextBox;
	// 게임 참가 버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;
	
	// 이름 최대 길이
	UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	int32 NameMaxLength;
};
