// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInfoInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPlayerInfoInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CUBESHOOTER_API IPlayerInfoInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="PlayerInfo")
	int32 GetPlayerScore() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="PlayerInfo")
	FString GetPlayerDisplayName() const;
};
