// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShooterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CUBESHOOTER_API AShooterPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	
	AShooterPlayerState();
	
	// Getters
	FORCEINLINE FString GetPlayerName() const { return ShooterName; }
	FORCEINLINE int32 GetPlayerScore() const { return PlayerScore;}
	
	
	void SetPlayerScore(int32 NewScore);
	UFUNCTION(BlueprintCallable, Category = "Components|PlayerState")
	void SetPlayerName(const FString& NewName);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Called when PlayerName replicated
	UFUNCTION()
	void OnRep_ShooterName();
	
	// Called when PlayerName replicated
	UFUNCTION()
	void OnRep_PlayerScore();
	
	UPROPERTY(ReplicatedUsing=OnRep_ShooterName)
	FString ShooterName;
	UPROPERTY(ReplicatedUsing=OnRep_PlayerScore)
	int32 PlayerScore;
};
