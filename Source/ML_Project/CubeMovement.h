#pragma once

#include "CoreMinimal.h"
#include "NeuralNetwork.h"
#include "GameFramework/Character.h"
#include "CubeMovement.generated.h"

class UNeuralNetworkDataWidget;

UCLASS()
class ML_PROJECT_API ACubeMovement : public ACharacter
{
	GENERATED_BODY()

public:
	ACubeMovement();

	UPROPERTY(BlueprintReadWrite)
	UNeuralNetworkDataWidget* NeuralNetworkDataWidget = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	float Horizontal = 0.0f;
	float Vertical = 0.0f;
	FVector Movement = FVector::Zero();
	bool JsonSaved = false;

	UPROPERTY(EditAnywhere)
	float TimeDilation = 2.0f;
	
	UPROPERTY(EditAnywhere)
	AActor* Sphere = nullptr;

	UPROPERTY(EditAnywhere)
	uint32 NbInputs = 0;

	UPROPERTY(EditAnywhere)
	uint32 NbHidden = 0;
	
	UPROPERTY(EditAnywhere)
	uint32 NbOutputs = 0;

	UPROPERTY(EditAnywhere)
	double LearningRate = 0.001;

	UPROPERTY(EditAnywhere)
	double Momentum = 0.9;
	
	UPROPERTY(EditAnywhere)
	bool UseBatchLearning = false;

	UPROPERTY(EditAnywhere)
	double MaxEpochs = 200;
	
	FTrainingEntry Entry;
	FNeuronsConfiguration NeuronsConfiguration;
	FNetworkConfiguration NetworkConfiguration;

	FNeuralNetwork NeuralNetwork;

	void InitializeNeuralNetwork();
	void EntriesTick();
	void OutputsValuesTick(const bool TrainingOver);
};