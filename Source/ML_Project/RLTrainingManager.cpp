#include "RLTrainingManager.h"
#include "DrivingInteractor.h"
#include "DrivingPolicy.h"
#include "DrivingRLTrainer.h"

ARLTrainingManager::ARLTrainingManager()
{
	PrimaryActorTick.bCanEverTick = true;

	/*
	 * TODO : Instantiate SplineComp for Interactor and Trainer
	 * Possible solution make this and the rest in blueprint
	 * and get BP_Spline
	 * don't know if possible in C++
	 */
	
	DrivingInteractor = CreateDefaultSubobject<UDrivingInteractor>(TEXT("DrivingInteractor"));
	DrivingPolicy = CreateDefaultSubobject<UDrivingPolicy>(TEXT("DrivingPolicy"));
	DrivingRLTrainer = CreateDefaultSubobject<UDrivingRLTrainer>(TEXT("DrivingRLTrainer"));
}

void ARLTrainingManager::BeginPlay()
{
	Super::BeginPlay();

	DrivingInteractor->SetupInteractor();
	DrivingPolicy->SetupPolicy(DrivingInteractor, FLearningAgentsPolicySettings(), LearningAgentsNeuralNetwork);
	DrivingRLTrainer->SetupTrainer(DrivingInteractor, DrivingPolicy);
}

void ARLTrainingManager::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	DrivingRLTrainer->RunTraining(FLearningAgentsTrainerTrainingSettings(), FLearningAgentsTrainerGameSettings(),
		FLearningAgentsTrainerPathSettings(), FLearningAgentsCriticSettings(), false);
}