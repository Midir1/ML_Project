// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NeuralNetworkDataWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ML_PROJECT_API UNeuralNetworkDataWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DistanceText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* OutputText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TrainingText = nullptr;

	void SetWidgetData(FText const& Distance, FText const& Output, FText const& Training) const;
};
