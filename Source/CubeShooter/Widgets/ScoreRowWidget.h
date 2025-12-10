// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreRowWidget.generated.h"

/**
 * 
 */

class UTextBlock;
UCLASS()
class CUBESHOOTER_API UScoreRowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	void SetPlayerInfo(const FString& PlayerName, int32 PlayerScore, FLinearColor Color);
 
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;
 
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerScoreText;
};
