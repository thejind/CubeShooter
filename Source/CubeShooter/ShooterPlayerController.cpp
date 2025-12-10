// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterPlayerController.h"
#include "ShooterPlayerState.h"
#include "PlayerCube.h"


void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShooterPlayerController::SpawnPlayerCube()
{
	if (!HasAuthority() || OwnedCube) return;
 
	FVector LocalSpawnLocation = GetPawn() ? GetPawn()->GetActorLocation() + FVector(200,0,0) : FVector(0,0,200);
	FRotator SpawnRotation = FRotator::ZeroRotator;
 
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetPawn();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	
	OwnedCube = GetWorld()->SpawnActor<APlayerCube>(APlayerCube::StaticClass(), LocalSpawnLocation, SpawnRotation, SpawnParams);
	if (OwnedCube)
	{
		APlayerState* PS = GetPlayerState<APlayerState>();
		int32 PlayerId = PS ? PS->GetPlayerId() : 0;
		FLinearColor UniqueColor = FLinearColor::MakeFromHSV8((PlayerId * 50) % 255, 255, 255);
		OwnedCube->SetCubeColor(UniqueColor);
	}
}
