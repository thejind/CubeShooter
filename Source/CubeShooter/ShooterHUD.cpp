// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterHUD.h"
#include "Engine/World.h"
#include "CubeShooter/Widgets/MainWidget.h"
#include "CubeShooter/Widgets/ScoreboardWidget.h"
#include "TimerManager.h"

void AShooterHUD::BeginPlay()
{
	Super::BeginPlay();

			
	
	if (ScoreboardWidgetClass)
	{
		
		ScoreboardWidget = CreateWidget<UScoreboardWidget>(GetWorld(), ScoreboardWidgetClass);
		if (ScoreboardWidget)
		{
			ScoreboardWidget->AddToViewport();
 
			// Initial population
			
 
			// Periodically update scoreboard UI every second
			GetWorld()->GetTimerManager().SetTimer(ScoreUpdateTimerHandle, this, &ThisClass::UpdateScoreboard, 1.0f, true);
		}
	}
}

void AShooterHUD::UpdateScoreboard()
{
	if (ScoreboardWidget)
	{
		ScoreboardWidget->UpdateScoreboard();
	}
}
	
