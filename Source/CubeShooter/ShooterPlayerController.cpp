// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterPlayerController.h"
#include "ShooterPlayerState.h"
#include "CoreMinimal.h"
#include "CubeShooter/Interfaces/PlayerInfoInterface.h"
#include "PlayerCube.h"


AShooterPlayerController::AShooterPlayerController()
	: OwnedCube(nullptr)
{
	
}

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void AShooterPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void AShooterPlayerController::SpawnPlayerCube(FLinearColor UniqueColor, const FVector& Location, const FRotator& Rotation)
{
	
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetPawn();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
 
	if (!PlayerCubeClass)
		return;
	
	OwnedCube = GetWorld()->SpawnActor<APlayerCube>(PlayerCubeClass, Location, Rotation, SpawnParams);
	if (OwnedCube)
	{
		APlayerState* PS = GetPlayerState<APlayerState>();
		if (!PS)
		{
			UE_LOG(LogTemp, Warning, TEXT("SpawnPlayerCube: PlayerState not found for PlayerController %s"), *GetName());
			return;
		}
		
		OwnedCube->SetCubeColor(UniqueColor);
 
		if (PS->GetClass()->ImplementsInterface(UPlayerInfoInterface::StaticClass()))
		{
			IPlayerInfoInterface::Execute_SetPlayerColor(PS, UniqueColor);
		}
		
	}
}
