// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterHUD.generated.h"


class UScoreboardWidget;
class UMainWidget;

/**
 * 
 */
UCLASS()
class CUBESHOOTER_API AShooterHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
 
protected:
	
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UScoreboardWidget> ScoreboardWidgetClass;
	
	
	UPROPERTY()
	UScoreboardWidget* ScoreboardWidget;
 
	FTimerHandle ScoreUpdateTimerHandle;
 
	void UpdateScoreboard();
};
