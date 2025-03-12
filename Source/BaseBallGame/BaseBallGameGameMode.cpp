#include "BaseBallGameGameMode.h"

#include "FunctionLibrary/ComparingNumbersLib.h"
#include "FunctionLibrary/RandomNumberLib.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BaseBallGamePlayerController.h"

ABaseBallGameGameMode::ABaseBallGameGameMode(): TargetNumberLength(0)
{
	PlayerControllerClass = ABaseBallGamePlayerController::StaticClass();
}

void ABaseBallGameGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// 서버일 경우
	if (UKismetSystemLibrary::IsServer(GetWorld()))
	{
		// 난수 생성(1 ~ 9 범위)
		TargetNumber = URandomNumberLib::GenerateRandomNumber(TargetNumberLength);

		UE_LOG(LogTemp, Warning, TEXT("TargetNumber: %s"), *TargetNumber);
	}
}

void ABaseBallGameGameMode::GotMessageFromClient_Implementation(const FString& UserID, const FString& InputText) const
{
	// 모든 플레이어 찾기
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseBallGamePlayerController::StaticClass(), Actors);

	// 숫자 비교
	const FString CompareResult = UComparingNumbersLib::ComparingNumbers(InputText, TargetNumber);
	// 메세지 생성
	const FString NewMessage = FString::Printf(TEXT("%s가 입력한 %s의 결과: %s"), *UserID, *InputText, *CompareResult);
	
	for (AActor*& Actor: Actors)
	{
		if (const ABaseBallGamePlayerController* Controller = Cast<ABaseBallGamePlayerController>(Actor))
		{
			// 플레이어에게 결과 전달
			Controller->GotBroadcastMessage(NewMessage);
		}
	}
}
