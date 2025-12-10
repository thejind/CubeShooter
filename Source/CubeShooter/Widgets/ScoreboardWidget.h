// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CubeShooter/Widgets/ScoreRowWidget.h"
#include "ScoreboardWidget.generated.h"


class UVerticalBox;
class UScoreRowWidget;
/**
 * 
 */


UCLASS()
class CUBESHOOTER_API UScoreboardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateScoreboard();
 
protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ScoreListBox;
 
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UScoreRowWidget> ScoreRowWidgetClass;
	
};
