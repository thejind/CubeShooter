// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameMode.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "PhysicsCube.h"
#include "PlayerCube.h"
#include "ShooterPlayerController.h"

void AShooterGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	
	
}

void AShooterGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	if (AShooterPlayerController* PC = Cast<AShooterPlayerController>(Exiting))
	{
		if (PC->OwnedCube)
		{
			PC->OwnedCube->Destroy();
			PC->OwnedCube = nullptr;
		}
	}
}

void AShooterGameMode::BeginPlay()
{
	Super::BeginPlay();
	//
	if (HasAuthority() && PhysicsCubeClass)
	{
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ThisClass::SpawnPhysicsCube, SpawnInterval, true, 2.f);
	}
}

void AShooterGameMode::SpawnPhysicsCube()
{
	if (!PhysicsCubeClass)
		return;
 
	FVector SpawnLocation = FVector(FMath::RandRange(-500, 500), FMath::RandRange(-500, 500), 300.f);
	FRotator SpawnRotation = FRotator::ZeroRotator;
 
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
 
	GetWorld()->SpawnActor<APhysicsCube>(PhysicsCubeClass, SpawnLocation, SpawnRotation, Params);
}
