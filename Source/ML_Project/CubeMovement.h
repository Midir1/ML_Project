#pragma once

#include "CoreMinimal.h"
#include "NeuralNetwork.h"
#include "GameFramework/Character.h"
#include "CubeMovement.generated.h"

struct FInputActionValue;
class UInputComponent;

UCLASS()
class ML_PROJECT_API ACubeMovement : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction = nullptr;

public:
	ACubeMovement();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);

private:
	float Horizontal = 0.0f;
	float Vertical = 0.0f;
	FVector Movement = FVector::Zero();

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

	UPROPERTY(EditAnywhere)
	double DesiredAccuracy = 90;
	
	std::vector<FNeuralNetwork::FTrainingEntry> Entries;
	FNeuralNetwork::FTrainingData Data;

	FNeuralNetwork ThisNeuralNetwork;

	void NeuralNetworkMain();
};