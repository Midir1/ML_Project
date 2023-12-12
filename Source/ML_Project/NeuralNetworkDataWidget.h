#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NeuralNetworkDataWidget.generated.h"

UCLASS(Abstract)
class ML_PROJECT_API UNeuralNetworkDataWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DistanceText = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Output1Text = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Output2Text = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TrainingText = nullptr;

	void SetWidgetData(const float Distance, const float Output1, const float Output2, const bool Training) const;

private:
	const FLinearColor RedColor = FLinearColor::Red;
	const FLinearColor GreenColor = FLinearColor::Green;
	FSlateColor RedSlateColor = FSlateColor(RedColor);
	FSlateColor GreenSlateColor = FSlateColor(GreenColor);
};