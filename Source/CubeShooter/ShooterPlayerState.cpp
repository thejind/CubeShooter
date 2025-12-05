// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerState.h"
#include "Net/UnrealNetwork.h"

AShooterPlayerState::AShooterPlayerState()
{
	bReplicates = true;
	PlayerScore = 0;
	DisplayName = "Player Name";
}

void AShooterPlayerState::AddPlayerScore(int32 Delta)
{
	if (HasAuthority())
	{
		PlayerScore += Delta;
		ForceNetUpdate();

		UE_LOG(LogTemp, Log, TEXT("Player %s new score: %d"), *DisplayName, PlayerScore);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AddScore called on client, this should be server-only!"));
	}
		
}

void AShooterPlayerState::SetDisplayName(const FString& NewName)
{
	if (HasAuthority())
	{
		DisplayName = NewName;
		GEngine->AddOnScreenDebugMessage(
			-1, 3.0f, FColor::Green,
			FString::Printf(TEXT("Name updated in Player State: %s"), *DisplayName)
		);
		OnRep_DisplayName();
	}
	else
	{
		// If this gets called on client by mistake
		UE_LOG(LogTemp, Warning, TEXT("SetPlayerDisplayName called on client, this should be server-only!"));
	}
}

void AShooterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AShooterPlayerState, DisplayName);
	DOREPLIFETIME(AShooterPlayerState, PlayerScore);
}

void AShooterPlayerState::OnRep_DisplayName()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 3.0f, FColor::Green,
			FString::Printf(TEXT("Name updated: %s"), *DisplayName)
		);
		
		OnPlayerNameChanged.Broadcast(DisplayName);
	}
}

void AShooterPlayerState::OnRep_PlayerScore()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 3.0f, FColor::Yellow,
			FString::Printf(TEXT("%s Score: %d"), *DisplayName, PlayerScore)
		);
	}
}

void AShooterPlayerState::OnRep_PlayerColor()
{
}
