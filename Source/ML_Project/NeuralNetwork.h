#pragma once

#include "Structs.h"

class FNeuralNetwork
{
public:
	//Constructors used for initialization
	FNeuralNetwork();
	explicit FNeuralNetwork(FNeuronsConfiguration const& NeuronsConfiguration,
		FNetworkConfiguration const& NetworkConfiguration);
	explicit FNeuralNetwork(FNeuronsConfiguration const& NeuronsConfiguration,
		FNetworkConfiguration const& NetworkConfiguration, TArray<double> const& Weights);

	void Train(FTrainingData const& TrainingData);
	
	double GetOutputsValuesClamped() const
	{
		return OutputsValuesClamped.Last();
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

	TArray<double> InputsValues;
	TArray<double> HiddenValues;
	TArray<double> OutputsValues;
	TArray<double> OutputsValuesClamped;

	TArray<double> InputsWeights;
	TArray<double> HiddenWeights;

	TArray<double> DeltaInputsWeights;
	TArray<double> DeltaHiddenWeights;

	TArray<double> ErrorGradientsHidden;
	TArray<double> ErrorGradientsOutputs;
	
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

	//void InitializeNbNeurons(FNeuronsConfiguration const NeuronsConfiguration);
	void InitializeNetwork();
	//void InitializeNetworkConfiguration(FNetworkConfiguration const NetworkConfiguration);
	void InitializeWeights();
	void LoadWeights(TArray<double> const& Weight);

	TArray<double> const& Evaluate(TArray<double> const& Input);

	double GetHiddenErrorGradient(uint32 HiddenIndex) const;

	void RunEpoch(FTrainingSet const& TrainingSet);
	void Backpropagation(TArray<double> const& ExpectedOutputs);
	void UpdateWeights();

	void GetSetAccuracyAndMse(FTrainingSet const& TrainingSet, double& Accuracy, double& Mse);

	//Sigmoïd function for neuron activation
	static double Sigmoid(const double X)
	{
		return 1.0 / (1.0 + exp(-X));
	}

	//Clamp value because of sigmoïd asymptotic nature (can't reach 0 and 1 values)
	static double ClampOutputValue(const double X)
	{
		if (X < 0.1) return 0.0;
		if (X > 0.9) return 1.0;
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
	TArray<double> const& GetInputsWeights() const
	{
		return InputsWeights;
	}

	TArray<double> const& GetHiddenWeights() const
	{
		return HiddenWeights;
	}

	static double GetOutputErrorGradient(const double DesiredValue, const double OutputValue)
	{
		return OutputValue * (1.0 - OutputValue) * (DesiredValue - OutputValue);
	}
};