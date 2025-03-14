#include "NumberBaseballGameMode.h"

#include "NumberBaseballGameState.h"
#include "FunctionLibrary/ComparingNumbersLib.h"
#include "FunctionLibrary/RandomNumberLib.h"
#include "Kismet/GameplayStatics.h"
#include "Player/NumberBaseballPlayerController.h"

ANumberBaseballGameMode::ANumberBaseballGameMode(): TargetNumberLength(0)
{
	PlayerControllerClass = ANumberBaseballPlayerController::StaticClass();
}

void ANumberBaseballGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// 난수 생성(1 ~ 9 범위)
	TargetNumber = URandomNumberLib::GenerateRandomNumber(TargetNumberLength);

	UE_LOG(LogTemp, Warning, TEXT("TargetNumber: %s"), *TargetNumber);
	
	// 생성된 숫자의 길이 게임 스테이트에 저장
	if (ANumberBaseballGameState* NumberBaseballGameState = GetGameState<ANumberBaseballGameState>())
	{
		NumberBaseballGameState->SetTargetNumberLength(TargetNumberLength);
	}

}

bool ANumberBaseballGameMode::Server_GotInputText_Validate(const FString& UserID, const FString& InputText)
{
	// 필요시 유효성 체크 로직 구현
	return true;
}

void ANumberBaseballGameMode::Server_GotInputText_Implementation(const FString& UserID, const FString& InputText) const
{
	// 모든 플레이어 찾기
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANumberBaseballPlayerController::StaticClass(), Actors);

	// 숫자 비교
	const FString CompareResult = UComparingNumbersLib::ComparingNumbers(InputText, TargetNumber);
	// 메세지 생성
	const FString NewMessage = FString::Printf(TEXT("%s가 입력한 %s의 결과: %s"), *UserID, *InputText, *CompareResult);
	
	for (AActor*& Actor: Actors)
	{
		if (const ANumberBaseballPlayerController* Controller = Cast<ANumberBaseballPlayerController>(Actor))
		{
			// 플레이어에게 결과 전달
			Controller->Client_GotInputText(NewMessage);
		}
	}
}
