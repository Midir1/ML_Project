#include "NeuralNetworkDataWidget.h"
#include "Components/TextBlock.h"

void UNeuralNetworkDataWidget::SetWidgetData(const float Distance, const float Output, const bool Training) const
{
	DistanceText->SetText(FText::FromString(FString::SanitizeFloat(Distance)));
	OutputText->SetText(FText::FromString(FString::SanitizeFloat(Output)));
	
	if(!Training)
	{
		TrainingText->SetColorAndOpacity(RedSlateColor);
		TrainingText->SetText(FText::FromString("Training In Progress"));
	}
	else
	{
		TrainingText->SetColorAndOpacity(GreenSlateColor);
		TrainingText->SetText(FText::FromString("Training Done"));
	}
}