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
	bOnlyRelevantToOwner = false;
	bNetLoadOnClient = true;
	NetDormancy = DORM_Awake;
}

void APlayerCube::OnRep_Color()
{
	if (MeshComponent && BaseMaterial)
	{
		UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		if (DynMaterial)
		{
			DynMaterial->SetVectorParameterValue("BaseColor", CubeColor);
			MeshComponent->SetMaterial(0, DynMaterial);
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



