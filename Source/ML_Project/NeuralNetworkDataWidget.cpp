#include "NeuralNetworkDataWidget.h"
#include "Components/TextBlock.h"

void UNeuralNetworkDataWidget::SetWidgetData(const float Distance, const float Output1, const float Output2,
	const bool Training) const
{
	DistanceText->SetText(FText::FromString(FString::SanitizeFloat(Distance)));
	Output1Text->SetText(FText::FromString(FString::SanitizeFloat(Output1)));
	Output2Text->SetText(FText::FromString(FString::SanitizeFloat(Output2)));
	
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