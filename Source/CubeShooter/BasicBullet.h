// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicBullet.generated.h"

class UProjectileMovementComponent;

UCLASS()
class CUBESHOOTER_API ABasicBullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABasicBullet();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BulletMesh;
 
	void FireInDirection(const FVector& ShootDirection);
 
protected:
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;
};
