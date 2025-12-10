// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCube.generated.h"

UCLASS()
class CUBESHOOTER_API APlayerCube : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerCube();
 
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;
 
	UPROPERTY(ReplicatedUsing=OnRep_Color)
	FLinearColor CubeColor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	UMaterialInterface* BaseMaterial;
	
	UFUNCTION()
	void OnRep_Color();
 
	void SetCubeColor(const FLinearColor& NewColor);
 
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
};
