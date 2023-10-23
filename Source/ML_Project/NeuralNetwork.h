#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeuralNetwork.generated.h"

UCLASS()
class ML_PROJECT_API ANeuralNetwork : public AActor
{
	GENERATED_BODY()

	struct FTrainingEntry
	{
		std::vector<double> Inputs;
		std::vector<uint32> ExpectedOutputs;
	};

	typedef std::vector<FTrainingEntry> FTrainingSet;

	struct FTrainingData
	{
		FTrainingSet TrainingSet;
		FTrainingSet GeneralizationSet;
		FTrainingSet ValidationSet;
	};
	
public:
	
	//Basic struct to declare numbers of neurons in InputsLayer, HiddenLayer and OutputsLayer
	struct FNeuronsConfiguration
	{
		uint32 NbInputs;
		uint32 NbHidden;
		uint32 NbOutputs;
	} NeuronsConfiguration;

	struct FNetworkConfiguration
	{
		// Learning params
		double LearningRate = 0.001;
		double Momentum = 0.9;
		bool UseBatchLearning = false;

		// Stopping conditions
		uint32 MaxEpochs = 150;
		double DesiredAccuracy = 90;
	} NetworkConfiguration;

	//Constructors for initialization
	ANeuralNetwork();
	explicit ANeuralNetwork(std::vector<double> const& Weights);

	void Train(FTrainingData const& TrainingData);

private:

	//Neurons
	uint32 NbInputs;
	uint32 NbHidden;
	uint32 NbOutputs;

	std::vector<double> InputsValues;
	std::vector<double> HiddenValues;
	std::vector<double> OutputsValues;
	std::vector<int32> OutputsValuesClamped;

	std::vector<double> InputsWeights;
	std::vector<double> HiddenWeights;

	std::vector<double> DeltaInputsWeights;
	std::vector<double> DeltaHiddenWeights;

	std::vector<double> ErrorGradientsHidden;
	std::vector<double> ErrorGradientsOutputs;
	
	//Training Settings
	double LearningRate;
	double Momentum;
	double DesiredAccuracy;
	uint32 MaxEpochs;
	bool UseBatchLearning;

	uint32 CurrentEpoch;  
	double TrainingSetAccuracy;
	double ValidationSetAccuracy;
	double GeneralizationSetAccuracy;
	double TrainingSetMse;
	double ValidationSetMse;
	double GeneralizationSetMse;

	void InitializeNbNeurons();
	void InitializeNetwork();
	void InitializeWeights();
	void LoadWeights(std::vector<double> const& Weight);

	std::vector<int32> const& Evaluate(std::vector<double> const& Input);

	double GetHiddenErrorGradient(uint32 HiddenIndex) const;

	void RunEpoch(FTrainingSet const& TrainingSet);
	void Backpropagation(std::vector<uint32> const& ExpectedOutputs);
	void UpdateWeights();

	void GetSetAccuracyAndMse(FTrainingSet const& TrainingSet, double& Accuracy, double& Mse);

	//Sigmoïd function for neuron activation
	static double Sigmoid(const double X)
	{
		return 1.0 / (1.0 + exp(-X));
	}

	//Clamp value because of sigmoïd asymptotic nature (can't reach 0 and 1 values)
	static int32 ClampOutputValue(const double X)
	{
		if (X < 0.1) return 0;
		if (X > 0.9) return 1;
		return X;
	}

	//Index Accessors
	uint32 GetInputWeightIndex(const uint32 InputIndex, const uint32 HiddenIndex) const
	{
		return InputIndex * NbHidden + HiddenIndex;
	}

	uint32 GetHiddenWeightIndex(const uint32 HiddenIndex, const uint32 OutputIndex) const
	{
		return HiddenIndex * NbOutputs + OutputIndex;
	}

	//Weights Accessors
	std::vector<double> const& GetInputsWeights() const
	{
		return InputsWeights;
	}

	std::vector<double> const& GetHiddenWeights() const
	{
		return HiddenWeights;
	}

	static double GetOutputErrorGradient(const double DesiredValue, const double OutputValue)
	{
		return OutputValue * (1.0 - OutputValue) * (DesiredValue - OutputValue);
	}
};