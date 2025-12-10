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
	
	virtual void BeginPlay() override;
	
	void SpawnPlayerCube();
	
public:
	UPROPERTY()
	APlayerCube* OwnedCube;
	

	
};
