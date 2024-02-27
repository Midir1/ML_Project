#pragma once

#include "CoreMinimal.h"
#include "LearningAgentsInteractor.h"
#include "DrivingInteractor.generated.h"

class UPositionObservation;

UCLASS()
class ML_PROJECT_API UDrivingInteractor : public ULearningAgentsInteractor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FVector GoalLocation;

protected:
	virtual void SetupObservations_Implementation() override;
	virtual void SetObservations_Implementation(const TArray<int32>& AgentIds) override;

private:
	UPositionObservation* GoalLocationObservation = nullptr;
};