// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameInstance.h"
#include "MultiplayerManager.h"

void UShooterGameInstance::HostSessionWithCode(const FString& JoinCode)
{
	if (!MultiplayerManager)
	{
		MultiplayerManager = NewObject<UMultiplayerManager>(this);
	}
	MultiplayerManager->HostSession(JoinCode);
}

void UShooterGameInstance::JoinSessionWithCode(const FString& JoinCode)
{
	if (!MultiplayerManager)
	{
		MultiplayerManager = NewObject<UMultiplayerManager>(this);
	}
	MultiplayerManager->JoinSession(JoinCode);
}
