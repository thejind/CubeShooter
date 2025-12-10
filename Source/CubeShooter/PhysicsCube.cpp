// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsCube.h"

#include "ShooterPlayerState.h"
#include "GameFramework/PlayerState.h"
#include "CubeShooter/Interfaces/PlayerInfoInterface.h"

// Sets default values
APhysicsCube::APhysicsCube()
{
	bReplicates = true;
	SetReplicateMovement(true);
	
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = CubeMesh;
	
	
	CubeMesh->SetSimulatePhysics(true);
	CubeMesh->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void APhysicsCube::BeginPlay()
{
	Super::BeginPlay();
	Tags.AddUnique(FName(TEXT("PhysicsActor")));
	SetLifeSpan(10.f);
}



void APhysicsCube::Multicast_DestroyCube_Implementation()
{
	Destroy();
}

void APhysicsCube::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APhysicsCube::OnShot(APlayerState* Shooter)
{
	if (!HasAuthority())
		return;
 
	Multicast_DestroyCube();
 
	if (Shooter && Shooter->GetClass()->ImplementsInterface(UPlayerInfoInterface::StaticClass()))
	{
		AShooterPlayerState* SPS = Cast<AShooterPlayerState>(Shooter); 
		SPS->AddPlayerScore(1);// Increment player score
	}
}


