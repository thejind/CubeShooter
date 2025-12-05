// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "MultiplayerManager.generated.h"

/**
 * 
 */
UCLASS()
class CUBESHOOTER_API UMultiplayerManager : public UObject
{
	GENERATED_BODY()
	
public:
	UMultiplayerManager();
 
	// Host a session with a join code
	UFUNCTION(BlueprintCallable, Category="Multiplayer")
	void HostSession(const FString& JoinCode);
 
	// Find and join a session by join code
	UFUNCTION(BlueprintCallable, Category="Multiplayer")
	void JoinSession(const FString& JoinCode);
	
	void MapToJoinObjectLocation(const FString& Map);
	
	FString MapToJoin;
	
private:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
 
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	FString PendingJoinCode;
 
	IOnlineSessionPtr GetSessionInterface() const;
	
};
