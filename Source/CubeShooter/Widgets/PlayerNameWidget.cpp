// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerNameWidget.h"
#include "Components/TextBlock.h"

void UPlayerNameWidget::SetDisplayName(const FString& NewName)
{
	GEngine->AddOnScreenDebugMessage(
			-1, 3.0f, FColor::Green,
			FString::Printf(TEXT("UI Fnc Called"))
		);
	if (PlayerNameText)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 3.0f, FColor::Green,
			FString::Printf(TEXT("UI Fnc Called"))
		);
		PlayerNameText->SetText(FText::FromString(NewName));
	}
}
