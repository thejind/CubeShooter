// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameMode.h"
#include "ShooterGameInstance.h"
#include "ShooterPlayerState.h"

void AShooterGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	GEngine->AddOnScreenDebugMessage(
			-1, 3.0f, FColor::Green,
			FString::Printf(TEXT("Post Login Called"))
		);
	
	if (!NewPlayer)
		return;
	
	FString PlayerName = TEXT("Unnamed Player");
	
	if (UGameInstance* GI = NewPlayer->GetGameInstance())
	{
		if (UShooterGameInstance* SGI = Cast<UShooterGameInstance>(GI))
		{
			PlayerName = SGI->PendingDisplayName;
		}
	}
	
	if (AShooterPlayerState* PS = NewPlayer->GetPlayerState<AShooterPlayerState>())
	{
		PS->SetPlayerName(PlayerName);
	}
	
}
