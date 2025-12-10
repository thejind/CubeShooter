// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerNameWidget.generated.h"
class UTextBlock;
/**
 * 
 */
UCLASS()
class CUBESHOOTER_API UPlayerNameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//Text Block for Name 
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PlayerNameText;
	
	// Setter for Display Name
	UFUNCTION(BlueprintCallable, Category="Player")
	void SetDisplayName(const FString& NewName);
};
