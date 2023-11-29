// Fill out your copyright notice in the Description page of Project Settings.


#include "NeuralNetworkDataWidget.h"

#include "Components/TextBlock.h"

void UNeuralNetworkDataWidget::SetWidgetData(FText const& Distance, FText const& Output, FText const& Training) const
{
	DistanceText->SetText(Distance);
	OutputText->SetText(Output);
	TrainingText->SetText(Training);
}