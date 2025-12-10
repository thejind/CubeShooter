// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreRowWidget.h"
#include "Components/TextBlock.h"

void UScoreRowWidget::SetPlayerInfo(const FString& PlayerName, int32 PlayerScore, FLinearColor Color)
{
	if (PlayerNameText)
	{
		PlayerNameText->SetText(FText::FromString(PlayerName));
		PlayerNameText->SetColorAndOpacity(Color);
	}
		
	if (PlayerScoreText)
		PlayerScoreText->SetText(FText::AsNumber(PlayerScore));
	
	
	
	
}
