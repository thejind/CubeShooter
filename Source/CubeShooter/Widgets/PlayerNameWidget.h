// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerNameWidget.generated.h"

/**
 * 
 */
UCLASS()
class CUBESHOOTER_API UPlayerNameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* PlayerNameText;
 
	UFUNCTION(BlueprintCallable, Category="Player")
	void SetDisplayName(const FString& NewName);
};
