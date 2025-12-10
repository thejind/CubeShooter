// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/PlayerInfoInterface.h"
#include "ShooterPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerNameChanged, const FString&, NewName);

/**
 * 
 */
UCLASS()
class CUBESHOOTER_API AShooterPlayerState : public APlayerState, public IPlayerInfoInterface
{
	GENERATED_BODY()
	
public:
	
	AShooterPlayerState();
	
	// Getters
	virtual int32 GetPlayerScore_Implementation() const override { return PlayerScore;}
	virtual FString GetPlayerDisplayName_Implementation() const override { return PlayerDisplayName;}
	virtual FLinearColor GetPlayerColor_Implementation() const override { return PlayerColor;}
	virtual void SetPlayerColor_Implementation(const FLinearColor& NewColor) override;
	
	void AddPlayerScore(int32 Delta);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetPlayerDisplayName(const FString& NewName);
	
	void SetPlayerDisplayName(const FString& NewName); // Server-only direct setter
	
	// Called when PlayerScore replicated
	UFUNCTION()
	void OnRep_PlayerDisplayName();
	
	// Called when PlayerScore replicated
	UFUNCTION()
	void OnRep_PlayerScore();
	
	UFUNCTION() 
	void OnRep_PlayerColor();
	
	UPROPERTY(ReplicatedUsing=OnRep_PlayerDisplayName)
	FString PlayerDisplayName;
	UPROPERTY(ReplicatedUsing=OnRep_PlayerScore)
	int32 PlayerScore;
	UPROPERTY(ReplicatedUsing = OnRep_PlayerColor) 
	FLinearColor PlayerColor;
};
