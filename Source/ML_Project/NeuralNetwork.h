﻿#pragma once

#include <vector>

struct FTrainingEntry
{
	std::vector<double> Inputs;
	std::vector<double> ExpectedOutputs;
};

typedef std::vector<FTrainingEntry> FTrainingSet;

struct FTrainingData
{
	FTrainingSet TrainingSet;
	FTrainingSet GeneralizationSet;
	FTrainingSet ValidationSet;
};
	
struct FNeuronsConfiguration
{
	uint32 NbInputs = 0;
	uint32 NbHidden = 0;
	uint32 NbOutputs = 0;
};

struct FNetworkConfiguration
{
	// Learning params
	double LearningRate = 0.001;
	double Momentum = 0.9;
	bool UseBatchLearning = false;

	// Stopping conditions
	uint32 MaxEpochs = 150;
	double DesiredAccuracy = 90;
};

class FNeuralNetwork
{
	
public:
	
	//Constructors used for initialization
	FNeuralNetwork();
	explicit FNeuralNetwork(FNeuronsConfiguration const& NeuronsConfiguration,
		FNetworkConfiguration const& NetworkConfiguration);
	explicit FNeuralNetwork(FNeuronsConfiguration const& NeuronsConfiguration,
		FNetworkConfiguration const& NetworkConfiguration, std::vector<double> const& Weights);

	void Train(FTrainingData const& TrainingData);

	//temp
	int32 GetOutputsValuesClamped() const
	{
		return OutputsValuesClamped.back();
	}

	uint32 GetNbOutputs() const
	{
		return NbOutputs;
	}

private:

	//Neurons
	uint32 NbInputs = 0;
	uint32 NbHidden = 0;
	uint32 NbOutputs = 0;

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
	double LearningRate = 0;
	double Momentum = 0;
	double DesiredAccuracy = 0;
	uint32 MaxEpochs = 0 ;
	bool UseBatchLearning = false;

	uint32 CurrentEpoch = 0;  
	double TrainingSetAccuracy = 0;
	double ValidationSetAccuracy = 0;
	double GeneralizationSetAccuracy = 0;
	double TrainingSetMse = 0;
	double ValidationSetMse = 0;
	double GeneralizationSetMse = 0;

	void InitializeNbNeurons(FNeuronsConfiguration const NeuronsConfiguration);
	void InitializeNetwork();
	void InitializeNetworkConfiguration(FNetworkConfiguration const NetworkConfiguration);
	void InitializeWeights();
	void LoadWeights(std::vector<double> const& Weight);

	std::vector<int32> const& Evaluate(std::vector<double> const& Input);

	double GetHiddenErrorGradient(uint32 HiddenIndex) const;

	void RunEpoch(FTrainingSet const& TrainingSet);
	void Backpropagation(std::vector<double> const& ExpectedOutputs);
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