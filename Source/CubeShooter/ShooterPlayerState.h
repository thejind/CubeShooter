// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShooterPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerNameChanged, const FString&, NewName);

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
	FORCEINLINE FString GetDisplayName() const { return DisplayName; }
	FORCEINLINE int32 GetPlayerScore() const { return PlayerScore;}
	
	UFUNCTION(BlueprintCallable, Category = "Components|PlayerState")
	void SetDisplayName(const FString& NewName);
	
	void AddPlayerScore(int32 Delta);
	
	FOnPlayerNameChanged OnPlayerNameChanged;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Called when PlayerName replicated
	UFUNCTION()
	void OnRep_DisplayName();
	
	// Called when PlayerName replicated
	UFUNCTION()
	void OnRep_PlayerScore();
	
	UFUNCTION() 
	void OnRep_PlayerColor();
	
	
	UPROPERTY(ReplicatedUsing=OnRep_DisplayName)
	FString DisplayName;
	UPROPERTY(ReplicatedUsing=OnRep_PlayerScore)
	int32 PlayerScore;
	UPROPERTY(ReplicatedUsing = OnRep_PlayerColor) 
	FLinearColor PlayerColor;
};
