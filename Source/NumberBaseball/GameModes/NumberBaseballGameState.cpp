PRAGMA_DISABLE_OPTIMIZATION

#include "NumberBaseballGameState.h"

#include "NumberBaseballGameMode.h"
#include "GameFramework/PlayerState.h"
#include "Manager/TurnManager.h"
#include "Net/UnrealNetwork.h"
#include "Player/NumberBaseballPlayerController.h"
#include "Player/NumberBaseballPlayerState.h"

void ANumberBaseballGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANumberBaseballGameState, JoinedPlayerIDs);
	DOREPLIFETIME(ANumberBaseballGameState, CurrentTurnPlayerIndex);
	DOREPLIFETIME(ANumberBaseballGameState, TurnManager);
	DOREPLIFETIME(ANumberBaseballGameState, CurrentRound);
}

void ANumberBaseballGameState::Server_JoinGame_Implementation()
{
	const int32 PlayerIndex = JoinedPlayerIDs.Num();
	
	if (ANumberBaseballPlayerController* NumberBaseballPlayerController = GetPlayerControllerByIndex(PlayerIndex))
	{
		const FUniqueNetIdRepl PlayerID = NumberBaseballPlayerController->PlayerState->GetUniqueId();
		if (!JoinedPlayerIDs.Contains(PlayerID))
		{
			// 플레이어 아이디 추가
			JoinedPlayerIDs.Add(PlayerID);
			
			JoinOtherPlayer();
		}
	}
}

bool ANumberBaseballGameState::Server_JoinGame_Validate()
{
	if (JoinedPlayerIDs.Num() == 2)
	{
		UE_LOG(LogTemp, Error, TEXT("더 이상 참가할 수 없습니다."));
		return false;
	}
	
	return true;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ANumberBaseballGameState::JoinOtherPlayer()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ANumberBaseballPlayerController* NumberBaseballPlayerController
			= Cast<ANumberBaseballPlayerController>(PlayerController))
		{
			NumberBaseballPlayerController->Client_JoinGame(JoinedPlayerIDs.Num() - 1);
		}
	}
}

void ANumberBaseballGameState::BindTurnManagerEvent()
{
	if (TurnManager)
	{
		TurnManager->OnTurnUpdated.AddDynamic(this, &ANumberBaseballGameState::OnTurnUpdated);
		TurnManager->OnTurnEnded.AddDynamic(this, &ANumberBaseballGameState::OnTurnEnded);
		TurnManager->OnTurnStarted.AddDynamic(this, &ANumberBaseballGameState::OnTurnStarted);
		TurnManager->OnTurnEndedImmediately.AddDynamic(this, &ANumberBaseballGameState::OnTurnEndedImmediately);
	}
}

void ANumberBaseballGameState::OnTurnStarted()
{
	for (const APlayerState* PlayerState : PlayerArray)
	{
		if (PlayerState)
		{
			if (ANumberBaseballPlayerController* NumberBaseballPlayerController
				= Cast<ANumberBaseballPlayerController>(PlayerState->GetPlayerController()))
			{
				// 현재 플레이어 턴 시작
				if (PlayerState->GetUniqueId() == JoinedPlayerIDs[CurrentTurnPlayerIndex])
				{
					NumberBaseballPlayerController->Client_OnTurnStarted();
				}
				// 이전 플레이어 턴 종료
				else
				{
					NumberBaseballPlayerController->Client_OnTurnEnded(true);
				}
			}
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ANumberBaseballGameState::OnTurnUpdated(const float RemainingTime)
{
	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		if (ANumberBaseballPlayerController* NumberBaseballPlayerController = GetPlayerControllerByIndex(i))
		{
			NumberBaseballPlayerController->Client_OnTurnUpdated(RemainingTime);
		}
	}
}

void ANumberBaseballGameState::OnTurnEnded()
{
	// 게임 시작 준비 턴일 경우
	if (TurnManager->GetCurrentTurnCount() == 0)
	{
		// 게임 시작
		TurnManager->StartTurn();
		OnTurnStarted();
		return;
	}

	Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode())->TurnTimeOver(CurrentTurnPlayerIndex);

	// 현재 턴 플레이어 턴 종료
	GetPlayerControllerByIndex(CurrentTurnPlayerIndex)->Client_OnTurnEnded(true);

	// 다음 턴 플레이어 설정
	CurrentTurnPlayerIndex++;
	CurrentTurnPlayerIndex %= 2;

	// 현재 최대 턴일 경우
	if (TurnManager->IsMaxTurn())
	{
		// 라운드 종료
		CurrentRound++;
		// 현재 최대 라운드일 경우
		if (CurrentRound == Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode())->GetMaxGameRound())
		{
			// TODO: 최종 게임 종료후 승/패 확인
		}
		else
		{
			// 3초 후 다음 라운드 시작
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(
				TimerHandle,
				this,
				&ANumberBaseballGameState::StartNextRound,
				3.0f,
				false
			);
		}
	}
	// 최대 턴이 아닐 경우
	else
	{
		ANumberBaseballPlayerController* PlayerController = GetPlayerControllerByIndex(CurrentTurnPlayerIndex);

		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda(
			[TurnManager = TurnManager, PlayerController = PlayerController]()
			{
				// 다음 턴 플레이어 턴 시작
				PlayerController->Client_OnTurnStarted();

				TurnManager->StartTurn();
			});

		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.0f, false);
	}
}

void ANumberBaseballGameState::OnTurnEndedImmediately()
{
	// 현재 턴 플레이어 턴 종료
	GetPlayerControllerByIndex(CurrentTurnPlayerIndex)->Client_OnTurnEnded(false);

	// 다음 턴 플레이어 설정
	CurrentTurnPlayerIndex++;
	CurrentTurnPlayerIndex %= 2;

	GetPlayerControllerByIndex(CurrentTurnPlayerIndex)->Client_OnTurnEnded(false);

	// 타이머 텍스트 초기화
	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		if (ANumberBaseballPlayerController* NumberBaseballPlayerController = GetPlayerControllerByIndex(i))
		{
			NumberBaseballPlayerController->Client_OnTurnUpdated(TurnManager->GetTurnDuration());
		}
	}
}

void ANumberBaseballGameState::StartNextRound() const
{
	TurnManager->PrepareTurnStart();
}

int32 ANumberBaseballGameState::GetIndexByPlayerID(const FUniqueNetIdRepl& PlayerID) const
{
	return JoinedPlayerIDs.Find(PlayerID);
}

ANumberBaseballPlayerController* ANumberBaseballGameState::GetPlayerControllerByUniqueID(
	const FUniqueNetIdRepl& PlayerID) const
{
	for (const APlayerState* PlayerState : PlayerArray)
	{
		if (PlayerState->GetUniqueId() == PlayerID)
		{
			return Cast<ANumberBaseballPlayerController>(PlayerState->GetPlayerController());
		}
	}
	
	return nullptr;
}

ANumberBaseballPlayerController* ANumberBaseballGameState::GetPlayerControllerByIndex(const int32 Index) const
{
	if (Index >= PlayerArray.Num())
	{
		return nullptr;
	}

	if (const ANumberBaseballPlayerState* NumberBaseballPlayerState
		= Cast<ANumberBaseballPlayerState>(PlayerArray[Index]))
	{
		if (ANumberBaseballPlayerController* NumberBaseballPlayerController
			= NumberBaseballPlayerState->GetNumberBaseballPlayerController())
		{
			return NumberBaseballPlayerController;
		}
	}

	return nullptr;
}

void ANumberBaseballGameState::OnRep_JoinedPlayerIDs()
{
	JoinOtherPlayer();
}
