// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerManager.h"

UMultiplayerManager::UMultiplayerManager()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
}

void UMultiplayerManager::HostSession(const FString& JoinCode)
{
	if (IOnlineSessionPtr SessionInterface = GetSessionInterface())
	{
		FOnlineSessionSettings Settings;
		Settings.bIsLANMatch = true;
		Settings.NumPublicConnections = 4;
		Settings.bShouldAdvertise = true;
		Settings.bUsesPresence = false;
		Settings.Set(FName("JOIN_CODE"), JoinCode, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
 
		SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
			FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMultiplayerManager::OnCreateSessionComplete)
		);
 
		SessionInterface->CreateSession(0, FName("GameSession"), Settings);
 
		UE_LOG(LogTemp, Log, TEXT("Hosting session with code: %s"), *JoinCode);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid session interface for hosting."));
	}
}

void UMultiplayerManager::JoinSession(const FString& JoinCode)
{
}

void UMultiplayerManager::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Log, TEXT("Session creation %s"), bWasSuccessful ? TEXT("succeeded") : TEXT("failed"));
	if (bWasSuccessful)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(this))
		{
			World->ServerTravel("/Game/Maps/YourMap?listen"); // Change to your map path
		}
	}
}

void UMultiplayerManager::OnFindSessionsComplete(bool bWasSuccessful)
{
}

void UMultiplayerManager::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
}

IOnlineSessionPtr UMultiplayerManager::GetSessionInterface() const
{
	if (const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
	{
		return OnlineSub->GetSessionInterface();
	}
	return nullptr;
}
