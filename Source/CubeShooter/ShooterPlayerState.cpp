// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerState.h"
#include "Net/UnrealNetwork.h"

AShooterPlayerState::AShooterPlayerState()
{
	bReplicates = true;
}

void AShooterPlayerState::SetPlayerScore(int32 PlayerScoreToAdd)
{
	
		
}

void AShooterPlayerState::SetPlayerName(const FString& NewName)
{
	if (HasAuthority())
	{
		ShooterName = NewName;
		OnRep_PlayerName();
	}
}

void AShooterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AShooterPlayerState, ShooterName);
	DOREPLIFETIME(AShooterPlayerState, PlayerScore);
}

void AShooterPlayerState::OnRep_ShooterName()
{
	
}

void AShooterPlayerState::OnRep_PlayerScore()
{
	
}
