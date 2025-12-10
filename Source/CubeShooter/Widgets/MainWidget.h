// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"
class UScoreboardWidget;
class USizeBox;
/**
 * 
 */
UCLASS()
class CUBESHOOTER_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	UScoreboardWidget* ScoreboardWidget;
	
	UPROPERTY(meta = (BindWidget))
	USizeBox* ScorePlaceholder;
	
public:
	
	void SetupScoreBoardWidget(UScoreboardWidget* WidgetPtr);
};
