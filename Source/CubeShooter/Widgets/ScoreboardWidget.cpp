// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreboardWidget.h"
#include "ScoreRowWidget.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameStateBase.h"
#include "CubeShooter/Interfaces/PlayerInfoInterface.h"
#include "GameFramework/PlayerState.h"

void UScoreboardWidget::UpdateScoreboard()
{
	if (!ScoreListBox || !ScoreRowWidgetClass)
		return;
 
	ScoreListBox->ClearChildren();
 
	UWorld* World = GetWorld();
	if (!World)
		return;
 
	AGameStateBase* GameState = World->GetGameState();
	if (!GameState)
		return;
 
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		if (!PlayerState)
			continue;
		
		
		if (PlayerState->GetClass()->ImplementsInterface(UPlayerInfoInterface::StaticClass()))
		{
			int32 PlayerScore = IPlayerInfoInterface::Execute_GetPlayerScore(PlayerState);
			FString PlayerName = IPlayerInfoInterface::Execute_GetPlayerDisplayName(PlayerState);
			FLinearColor PlayerColor = IPlayerInfoInterface::Execute_GetPlayerColor(PlayerState);
			
 
			UScoreRowWidget* PlayerRow = CreateWidget<UScoreRowWidget>(World, ScoreRowWidgetClass);
			if (PlayerRow)
			{
				PlayerRow->SetPlayerInfo(PlayerName, PlayerScore, PlayerColor);
				ScoreListBox->AddChild(PlayerRow);
				
			}
		}
	}
}
