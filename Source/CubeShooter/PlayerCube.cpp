// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCube.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
APlayerCube::APlayerCube()
{
	bReplicates = true;
 
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;
 
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(CubeMesh.Object);
	}
}

void APlayerCube::OnRep_Color()
{
	if (MeshComponent)
	{
		UMaterialInstanceDynamic* DynMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);
		if (DynMaterial)
		{
			DynMaterial->SetVectorParameterValue("BaseColor", CubeColor);
		}
	}
}

void APlayerCube::SetCubeColor(const FLinearColor& NewColor)
{
	CubeColor = NewColor;
	OnRep_Color();
}

void APlayerCube::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerCube, CubeColor);
}



