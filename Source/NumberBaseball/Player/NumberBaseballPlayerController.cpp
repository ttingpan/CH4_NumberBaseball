#include "NumberBaseballPlayerController.h"

#include "NumberBaseballGameState.h"
#include "NumberBaseballPlayerState.h"
#include "NumberBaseball/NumberBaseballGameMode.h"
#include "NumberBaseball/UI/ChatWidget.h"
#include "UI/NumberBaseballHUD.h"

void ANumberBaseballPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// UI 모드 설정
	SetInputMode(FInputModeUIOnly());
	// 마우스 커서 보이기
	bShowMouseCursor = true;

	// 서버일 경우 로그인
	if (HasAuthority())
	{
		Server_LogInPlayer();
	}
	// 클라이언트일 경우 위젯 초기화
	else
	{
		Client_InitWidget();
		Server_NotifyAllLogin();
	}
}

void ANumberBaseballPlayerController::Server_NotifyAllLogin_Implementation()
{
	if (ANumberBaseballGameState* GS = GetWorld()->GetGameState<ANumberBaseballGameState>())
	{
		GS->Multicast_UpdateUserIDs();
	}
}

void ANumberBaseballPlayerController::Server_RegisterUserID_Implementation(const FString& UserID)
{
	if (ANumberBaseballGameState* GS = GetWorld()->GetGameState<ANumberBaseballGameState>())
	{
		GS->RegisterUserID(UserID);
	}
}

void ANumberBaseballPlayerController::Client_UpdateOtherUserName_Implementation(const FString& OtherUserID)
{
	if (ChatWidget)
	{
		ChatWidget->SetOtherUserName(OtherUserID);
	}
}

void ANumberBaseballPlayerController::Client_GotInputText_Implementation(const FString& Message) const
{
	// 서버 스스로 호출하지 않도록 방어
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client_GotInputText: %s"), *Message);
	}
}

void ANumberBaseballPlayerController::Server_LogInPlayer_Implementation()
{
	FString UserID = "";
	// 아이디로 로그인
	if (ANumberBaseballPlayerState* NumberBaseballPlayerState = GetPlayerState<ANumberBaseballPlayerState>())
	{
		UserID = IsLocalPlayerController() ? "Host" : "Guest";
		NumberBaseballPlayerState->LogIn(UserID);

		UE_LOG(LogTemp, Warning, TEXT("%s 로그인"), *NumberBaseballPlayerState->GetUserID());

		// GameState에 등록
		Server_RegisterUserID(UserID);
	}

	// 위젯 초기화
	Client_InitWidget();
}

bool ANumberBaseballPlayerController::Server_LogInPlayer_Validate()
{
	return true;
}

void ANumberBaseballPlayerController::Client_InitWidget_Implementation()
{
	if (GetHUD())
	{
		// HUD 초기화
		if (ANumberBaseballHUD* NumberBaseballHUD = Cast<ANumberBaseballHUD>(GetHUD()))
		{
			NumberBaseballHUD->Init();

			ChatWidget = NumberBaseballHUD->GetChatWidget();
			if (ChatWidget)
			{
				// 커밋 델리게이트 바인딩
				ChatWidget->OnInputCommitted.AddDynamic(
					this, &ANumberBaseballPlayerController::Server_SetInputText);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NumberBaseballHUD가 존재하지 않습니다."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD가 존재하지 않습니다."));
	}
}

void ANumberBaseballPlayerController::Server_SetInputText_Implementation(const FString& InputText)
{
	if (const ANumberBaseballGameMode* GameMode = Cast<ANumberBaseballGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (const ANumberBaseballPlayerState* NumberBaseballPlayerState = GetPlayerState<ANumberBaseballPlayerState>())
		{
			GameMode->Server_GotInputText(NumberBaseballPlayerState->GetUserID(), InputText);
		}
	}
}

bool ANumberBaseballPlayerController::Server_SetInputText_Validate(const FString& InputText)
{
	// 필요시 유효성 체크 로직 구현
	return true;
}
