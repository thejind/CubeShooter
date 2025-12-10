// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/SizeBox.h"

void UMainWidget::SetupScoreBoardWidget(UScoreboardWidget* WidgetPtr)
{
	if (WidgetPtr)
		ScoreboardWidget = WidgetPtr;
	
}
