// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerNameWidget.h"
#include "Components/TextBlock.h"

void UPlayerNameWidget::SetDisplayName(const FString& NewName)
{
	
	if (PlayerNameText)
	{
		PlayerNameText->SetText(FText::FromString(NewName));
	}
}
