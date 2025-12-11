// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "ShooterPlayerController.generated.h"


class APlayerCube;
/**
 * 
 */
UCLASS()
class CUBESHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	AShooterPlayerController();
	virtual void BeginPlay() override;
	
	virtual void OnRep_PlayerState() override;
	
	
	
public:
	UPROPERTY()
	APlayerCube* OwnedCube;
	
	UPROPERTY(EditDefaultsOnly, Category = "CubeShooter")
	TSubclassOf<APlayerCube> PlayerCubeClass;
	
	void SpawnPlayerCube(FLinearColor UniqueColor, const FVector& Location, const FRotator& Rotation);
};
