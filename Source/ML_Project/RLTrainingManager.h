#pragma once

#include "CoreMinimal.h"
#include "LearningAgentsManager.h"
#include "RLTrainingManager.generated.h"

class UDrivingRLTrainer;
class UDrivingPolicy;
class UDrivingInteractor;
class ULearningAgentsNeuralNetwork;

UCLASS()
class ML_PROJECT_API ARLTrainingManager : public ALearningAgentsManager
{
	GENERATED_BODY()

public:
	ARLTrainingManager();

	UPROPERTY(EditAnywhere)
	ULearningAgentsNeuralNetwork* LearningAgentsNeuralNetwork;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UDrivingInteractor* DrivingInteractor = nullptr;
	UDrivingPolicy* DrivingPolicy = nullptr;
	UDrivingRLTrainer* DrivingRLTrainer = nullptr;
};