// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterPlayerController.h"
#include "ShooterPlayerState.h"
#include "CoreMinimal.h"
#include "CubeShooter/Interfaces/PlayerInfoInterface.h"
#include "PlayerCube.h"


void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnPlayerCube();
}

void AShooterPlayerController::SpawnPlayerCube()
{
	if (!HasAuthority() || OwnedCube)
		return;
 
	APlayerState* PS = GetPlayerState<APlayerState>();
	if (!PS)
	{
		// PlayerState not ready, retry after short delay to eliminate infinite recursion
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			SpawnPlayerCube();
		});
		return;
	}
 
	// Spawn location and params
	FVector LocalSpawnLocation = GetPawn() ? GetPawn()->GetActorLocation() + FVector(200, 0, 0) : FVector(0, 0, 200);
	FRotator SpawnRotation = FRotator::ZeroRotator;
 
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetPawn();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
 
	if (!PlayerCubeClass)
		return;
 
	OwnedCube = GetWorld()->SpawnActor<APlayerCube>(PlayerCubeClass, LocalSpawnLocation, SpawnRotation, SpawnParams);
	if (OwnedCube)
	{
		// Generate unique color seeded by PlayerId to ensure uniqueness
		int32 PlayerId = PS->GetPlayerId();
 
		// Use FRandomStream for seeded randomness
		FRandomStream Stream(PlayerId + 12345); // Any arbitrary seed offset
 
		float Hue = Stream.FRandRange(0.f, 360.f);
		FLinearColor UniqueColor = FLinearColor::MakeFromHSV8((uint8)Hue, 255, 255);
 
		OwnedCube->SetCubeColor(UniqueColor);
 
		// Update PlayerState via interface for replication
		if (PS->GetClass()->ImplementsInterface(UPlayerInfoInterface::StaticClass()))
		{
			IPlayerInfoInterface::Execute_SetPlayerColor(PS, UniqueColor);
		}
	}
}
