#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerSlotWidget.generated.h"

class UProgressBar;
class UBorder;
class UTextBlock;

UCLASS(Abstract)
class NUMBERBASEBALL_API UPlayerSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 위젯 초기화
	void InitWidget(bool bInIsMySlotWidget, int32 InWinScore);
	// 턴 업데이트
	void TurnUpdate(bool bIsMyTurn) const;
	// 플레이어 이름 설정
	void SetPlayerName(const FString& InPlayerName);
	// 점수 추가
	void UpdateScore(int32 Score) const;
	// 준비 완료 표시 설정
	void SetVisibilityReadyTextBorder(const bool bIsVisible) const;

private:
	UPROPERTY(meta = (BindWidget))
	UBorder* PlayerNameBorder;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* PlayerScoreProgressBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerScoreText;
	UPROPERTY(meta = (BindWidget))
	UBorder* ReadyTextBorder;

	// 자기 자신을 가리키는 위젯 여부
	bool bIsMySlotWidget;

	int32 WinScore;
};
