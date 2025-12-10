// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreRowWidget.h"
#include "Components/TextBlock.h"

void UScoreRowWidget::SetPlayerInfo(const FString& PlayerName, int32 PlayerScore)
{
	
	GEngine->AddOnScreenDebugMessage(
					-1, 10.0f, FColor::Red,
						FString::Printf(TEXT("Server Player Name %s and Player Score %d"), *PlayerName, PlayerScore)
				);
	if (PlayerNameText)
		PlayerNameText->SetText(FText::FromString(PlayerName));
	if (PlayerScoreText)
		PlayerScoreText->SetText(FText::AsNumber(PlayerScore));
}
