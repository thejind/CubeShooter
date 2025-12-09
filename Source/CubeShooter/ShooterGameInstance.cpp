// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameInstance.h"
#include "MultiplayerManager.h"

void UShooterGameInstance::Init()
{
	Super::Init();
	if (!MultiplayerManager)
	{
		MultiplayerManager = NewObject<UMultiplayerManager>(this);
	}
	
}

void UShooterGameInstance::Shutdown()
{
	Super::Shutdown();
	
}

void UShooterGameInstance::HostSessionWithCode(const FString& JoinCode)
{
	if (!MultiplayerManager)
	{
		MultiplayerManager = NewObject<UMultiplayerManager>(this);
	}
	MultiplayerManager->CreateSession();
}

void UShooterGameInstance::JoinSessionWithCode(const FString& JoinCode)
{
	if (!MultiplayerManager)
	{
		MultiplayerManager = NewObject<UMultiplayerManager>(this);
	}
	MultiplayerManager->JoinSession();
}

void UShooterGameInstance::SetPendingDisplayName(const FString& DisplayName)
{ 
	PendingDisplayName = DisplayName;
}


