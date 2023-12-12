#pragma once

#include "Structs.h"

class FNeuralNetwork
{
public:
	FNeuralNetwork();
	explicit FNeuralNetwork(FNeuronsConfiguration const& NeuronsConfiguration,
		FNetworkConfiguration const& NetworkConfiguration);
	explicit FNeuralNetwork(FNeuronsConfiguration const& NeuronsConfiguration,
		FNetworkConfiguration const& NetworkConfiguration, TArray<double> const& Weights);

	void Train(FTrainingEntry const& TrainingEntry);
	
	TArray<double> GetOutputsValuesClamped()
	{
		return OutputsValuesClamped;
	}

	TArray<double> GetInputsWeights()
	{
		return InputsWeights;
	}

	TArray<double> GetHiddenWeights()
	{
		return HiddenWeights;
	}

	bool IsTrainingDone() const
	{
		return TrainingDone;
	}

	uint32 GetNbInputs() const
	{
		return NbInputs;
	}

	uint32 GetNbHidden() const
	{
		return NbHidden;
	}

	uint32 GetNbOutputs() const
	{
		return NbOutputs;
	}
	
	TArray<double> const& GetInputsWeights() const
	{
		return InputsWeights;
	}

	TArray<double> const& GetHiddenWeights() const
	{
		return HiddenWeights;
	}

private:
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
	
	double LearningRate = 0;
	double Momentum = 0;
	uint32 MaxEpochs = 0;

	uint32 CurrentEpoch = 0;  
	bool TrainingDone = false;

	void InitializeNbNeurons(FNeuronsConfiguration const& NeuronsConfiguration);
	void InitializeNetwork();
	void InitializeNetworkConfiguration(FNetworkConfiguration const& NetworkConfiguration);
	void InitializeWeights();
	void LoadWeights(TArray<double> const& Weight);

	TArray<double> const& Evaluate(TArray<double> const& Input);

	double GetHiddenErrorGradient(uint32 HiddenIndex) const;

	void RunEpoch(FTrainingEntry const& TrainingEntry);
	void Backpropagation(TArray<double> const& ExpectedOutputs);
	void UpdateWeights();
	
	static double Sigmoid(const double X)
	{
		return 1.0 / (1.0 + exp(-X));
	}
	
	static double ClampOutputValue(const double X)
	{
		if (X < 0.1) return 0.0;
		if (X > 0.9) return 1.0;
		return X;
	}
	
	uint32 GetInputWeightIndex(const uint32 InputIndex, const uint32 HiddenIndex) const
	{
		return InputIndex * (NbHidden + 1) + HiddenIndex;
	}

	uint32 GetHiddenWeightIndex(const uint32 HiddenIndex, const uint32 OutputIndex) const
	{
		return HiddenIndex * NbOutputs + OutputIndex;
	}

	static double GetOutputErrorGradient(const double DesiredValue, const double OutputValue)
	{
		return OutputValue * (1.0 - OutputValue) * (DesiredValue - OutputValue);
	}
};