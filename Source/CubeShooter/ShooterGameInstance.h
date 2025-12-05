// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ShooterGameInstance.generated.h"

class UMultiplayerManager;

/**
 * 
 */
UCLASS()
class CUBESHOOTER_API UShooterGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="Multiplayer")
	void HostSessionWithCode(const FString& JoinCode);
 
	UFUNCTION(BlueprintCallable, Category="Multiplayer")
	void JoinSessionWithCode(const FString& JoinCode);
 
private:
	UPROPERTY()
	TObjectPtr<UMultiplayerManager> MultiplayerManager;
};
