// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerNameWidget.h"
#include "Components/TextBlock.h"

void UPlayerNameWidget::SetDisplayName(const FString& NewName)
{
	GEngine->AddOnScreenDebugMessage(
			-1, 10.f, FColor::Green,
			FString::Printf(TEXT("UI Fnc Called %s"), *NewName)
		);
	if (PlayerNameText)
	{
		PlayerNameText->SetText(FText::FromString(NewName));
	}
}
