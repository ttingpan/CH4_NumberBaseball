#include "NumberBaseballGameMode.h"

#include "NumberBaseballGameState.h"
#include "NumberBaseballPlayerState.h"
#include "FunctionLibrary/ComparingNumbersLib.h"
#include "FunctionLibrary/RandomNumberLib.h"
#include "Kismet/GameplayStatics.h"
#include "Player/NumberBaseballPlayerController.h"

ANumberBaseballGameMode::ANumberBaseballGameMode(): TargetNumberLength(0), RequiredReadyCount(2), ReadyCount(0),
                                                    LoginCount(0)
{
	PlayerControllerClass = ANumberBaseballPlayerController::StaticClass();
	GameStateClass = ANumberBaseballGameState::StaticClass();
	PlayerStateClass = ANumberBaseballPlayerState::StaticClass();
}

void ANumberBaseballGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ANumberBaseballGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer && NewPlayer->PlayerState)
	{
		LoginCount++;
		const FString PlayerName = FString::Printf(TEXT("Player%d"), LoginCount);
		NewPlayer->PlayerState->SetPlayerName(PlayerName);
		GetGameState<ANumberBaseballGameState>()->RegisterPlayerName(PlayerName);
	}
}

void ANumberBaseballGameMode::PlayerReady(const ANumberBaseballPlayerState* NumberBaseballPlayerState)
{
	const FString PlayerName = NumberBaseballPlayerState->GetPlayerName();
	FString LogText = FString::Printf(TEXT("플레이어 %s "), *PlayerName);
	if (NumberBaseballPlayerState->IsReady())
	{
		ReadyCount++;
		LogText += TEXT("준비 완료");
	}
	else
	{
		ReadyCount--;
		LogText += TEXT("준비 취소");
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *LogText);

	if (ReadyCount >= RequiredReadyCount)
	{
		StartGame();
	}
}

void ANumberBaseballGameMode::StartGame()
{
	UE_LOG(LogTemp, Warning, TEXT("게임 시작!"));
	// 난수 생성(1 ~ 9 범위)
	TargetNumber = URandomNumberLib::GenerateRandomNumber(TargetNumberLength);

	UE_LOG(LogTemp, Warning, TEXT("TargetNumber: %s"), *TargetNumber);

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ANumberBaseballPlayerController* NumberBaseballPlayerController = Cast<
			ANumberBaseballPlayerController>(It->Get()))
		{
			NumberBaseballPlayerController->GameStarted(TargetNumberLength);
		}
	}
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

	for (AActor*& Actor : Actors)
	{
		if (const ANumberBaseballPlayerController* Controller = Cast<ANumberBaseballPlayerController>(Actor))
		{
			// 플레이어에게 결과 전달
			Controller->Client_GotInputText(NewMessage);
		}
	}
}

bool ANumberBaseballGameMode::Server_GotInputText_Validate(const FString& UserID, const FString& InputText)
{
	// 필요시 유효성 체크 로직 구현
	return true;
}
