// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsCube.generated.h"

UCLASS()
class CUBESHOOTER_API APhysicsCube : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APhysicsCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CubeMesh;
 
	// Destroy cube on all clients
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DestroyCube();
 
	
 
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Called by server on player hit to award points and destroy
	void OnShot(class APlayerState* Shooter);
	
};
