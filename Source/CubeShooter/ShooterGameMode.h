// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ShooterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CUBESHOOTER_API AShooterGameMode : public AGameMode
{
	GENERATED_BODY()
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	virtual void Logout(AController* Exiting) override;
	
	virtual void BeginPlay() override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	TSubclassOf<class APhysicsCube> PhysicsCubeClass;
 
	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	float SpawnInterval = 5.f;
 
	FTimerHandle SpawnTimerHandle;
 
	void SpawnPhysicsCube();
	
	
};
