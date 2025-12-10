// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreRowWidget.h"
#include "Components/TextBlock.h"

void UScoreRowWidget::SetPlayerInfo(const FString& PlayerName, int32 PlayerScore)
{
	if (PlayerNameText)
		PlayerNameText->SetText(FText::FromString(PlayerName));
	if (PlayerScoreText)
		PlayerScoreText->SetText(FText::AsNumber(PlayerScore));
}
