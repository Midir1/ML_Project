#pragma once

#include "Structs.generated.h"

USTRUCT()
struct FTrainingEntry
{
	GENERATED_USTRUCT_BODY();
	
	TArray<double> Inputs;
	TArray<double> ExpectedOutputs;
};

USTRUCT()
struct FNeuronsConfiguration
{
	GENERATED_USTRUCT_BODY();
	
	uint32 NbInputs = 0;
	uint32 NbHidden = 0;
	uint32 NbOutputs = 0;
};

USTRUCT()
struct FNetworkConfiguration
{
	GENERATED_USTRUCT_BODY();
	
	// Learning params
	double LearningRate = 0.001;
	double Momentum = 0.9;
	bool UseBatchLearning = false;

	// Stopping conditions
	uint32 MaxEpochs = 150;
};

USTRUCT()
struct FWeights
{
	GENERATED_USTRUCT_BODY();
	
	TArray<double> Inputs;
	TArray<double> Hidden;
};