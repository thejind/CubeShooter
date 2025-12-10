// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerState.h"
#include "Net/UnrealNetwork.h"

AShooterPlayerState::AShooterPlayerState()
{
	bReplicates = true;
	PlayerDisplayName = TEXT("Unknown");
	PlayerScore = 0;
	PlayerColor = FLinearColor::Red;
}

void AShooterPlayerState::SetPlayerColor_Implementation(const FLinearColor& NewColor)
{
	if (HasAuthority())
	{
		PlayerColor = NewColor;
		OnRep_PlayerColor(); // update on server instantly
	}
}

void AShooterPlayerState::AddPlayerScore(int32 Delta)
{
	if (HasAuthority())
	{
		PlayerScore += Delta;
		OnRep_PlayerScore();

		UE_LOG(LogTemp, Log, TEXT("Player %s new score: %d"), *PlayerDisplayName, PlayerScore);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AddScore called on client, this should be server-only!"));
	}
		
}

void AShooterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AShooterPlayerState, PlayerDisplayName);
	DOREPLIFETIME(AShooterPlayerState, PlayerScore);
	DOREPLIFETIME(AShooterPlayerState, PlayerColor);
}

void AShooterPlayerState::ServerSetPlayerDisplayName_Implementation(const FString& NewName)
{
	SetPlayerDisplayName(NewName);
}

bool AShooterPlayerState::ServerSetPlayerDisplayName_Validate(const FString& NewName)
{
	return !NewName.IsEmpty(); // Basic validation
}


void AShooterPlayerState::SetPlayerDisplayName(const FString& NewName)
{
	if (HasAuthority() && PlayerDisplayName != NewName)
	{
		PlayerDisplayName = NewName;
		OnRep_PlayerDisplayName();
	}
}

void AShooterPlayerState::OnRep_PlayerDisplayName()
{
	
}

void AShooterPlayerState::OnRep_PlayerScore()
{
	
}

void AShooterPlayerState::OnRep_PlayerColor()
{
	
}
