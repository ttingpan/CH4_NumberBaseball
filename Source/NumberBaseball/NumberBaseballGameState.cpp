#include "NumberBaseballGameState.h"

#include "NumberBaseballPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Player/NumberBaseballPlayerController.h"

class ANumberBaseballPlayerState;

ANumberBaseballGameState::ANumberBaseballGameState()
	: TargetNumberLength(0)
{
}

void ANumberBaseballGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANumberBaseballGameState, TargetNumberLength);
}

void ANumberBaseballGameState::RegisterUserID(const FString& UserID)
{
	if (UserIDs.Num() < 2) // 최대 2명만 저장
	{
		UserIDs.Add(UserID);
		UE_LOG(LogTemp, Warning, TEXT("플레이어 등록: %s"), *UserID);
	}
}

void ANumberBaseballGameState::Multicast_UpdateUserIDs_Implementation()
{
	for (const APlayerState* PS : PlayerArray)
	{
		if (ANumberBaseballPlayerController* PC = Cast<ANumberBaseballPlayerController>(PS->GetPlayerController()))
		{
			PC->Client_UpdateOtherUserName(PC->GetPlayerState<ANumberBaseballPlayerState>()->GetUserID() == UserIDs[0]
				                               ? UserIDs[1]
				                               : UserIDs[0]);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("플레이어 컨트롤러가 존재하지 않습니다."));
		}
	}
}
