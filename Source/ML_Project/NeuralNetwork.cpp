#include "NeuralNetwork.h"
#include "Math/UnrealMathUtility.h"

FNeuralNetwork::FNeuralNetwork() {}

FNeuralNetwork::FNeuralNetwork(FNeuronsConfiguration const& NeuronsConfiguration,
                               FNetworkConfiguration const& NetworkConfiguration)
{
	InitializeNbNeurons(NeuronsConfiguration);
	InitializeNetwork();
	InitializeNetworkConfiguration(NetworkConfiguration);
	InitializeWeights();
}

FNeuralNetwork::FNeuralNetwork(FNeuronsConfiguration const& NeuronsConfiguration,
	FNetworkConfiguration const& NetworkConfiguration, TArray<double> const& Weights)
{
	InitializeNbNeurons(NeuronsConfiguration);
	InitializeNetwork();
	InitializeNetworkConfiguration(NetworkConfiguration);
	LoadWeights(Weights);
}

void FNeuralNetwork::Train(FTrainingEntry const& TrainingEntry)
{
	if(CurrentEpoch < MaxEpochs)
	{
		RunEpoch(TrainingEntry);
		CurrentEpoch++;
	}
	else if (!DoneValidationSet)
	{
		DoneValidationSet = true;
	}
}

TArray<double> const& FNeuralNetwork::Evaluate()
{
	for (uint32 HiddenIndex = 0; HiddenIndex < NbHidden; HiddenIndex++)
	{
		HiddenValues[HiddenIndex] = 0;
		
		for (uint32 InputIndex = 0; InputIndex <= NbInputs; InputIndex++)
		{
			uint32 const WeightIndex = GetInputWeightIndex(InputIndex, HiddenIndex);
			HiddenValues[HiddenIndex] += InputsValues[InputIndex] * InputsWeights[WeightIndex];
		}

		HiddenValues[HiddenIndex] = Sigmoid(HiddenValues[HiddenIndex]);
	}

	for (uint32 OutputIndex = 0; OutputIndex < NbOutputs; OutputIndex++)
	{
		OutputsValues[OutputIndex] = 0;
		
		for (uint32 HiddenIndex = 0; HiddenIndex <= NbHidden; HiddenIndex++)
		{
			uint32 const WeightIndex = GetHiddenWeightIndex(HiddenIndex, OutputIndex);
			OutputsValues[OutputIndex] += HiddenValues[HiddenIndex] * HiddenWeights[WeightIndex];
		}

		OutputsValues[OutputIndex] = Sigmoid(OutputsValues[OutputIndex]);
		OutputsValuesClamped[OutputIndex] = ClampOutputValue(OutputsValues[OutputIndex]);
	}

	return OutputsValuesClamped;
}

void FNeuralNetwork::InitializeNbNeurons(FNeuronsConfiguration const& NeuronsConfiguration)
{
	NbInputs = NeuronsConfiguration.NbInputs;
	NbHidden = NeuronsConfiguration.NbHidden;
	NbOutputs = NeuronsConfiguration.NbOutputs;
}

void FNeuralNetwork::InitializeNetwork()
{
	// Resizing the neurons values of InputLayer and HiddenLayer to add bias neurons
	InputsValues.SetNum(NbInputs + 1);
	HiddenValues.SetNum(NbHidden + 1);
	OutputsValues.SetNum(NbOutputs);
	OutputsValuesClamped.SetNum(NbOutputs);
	
	//Set bias values
	InputsValues.Last() = -1.0;
	HiddenValues.Last() = -1.0;
	
	InputsWeights.SetNum((NbInputs + 1) * (NbHidden + 1));
	HiddenWeights.SetNum((NbHidden + 1) * NbOutputs);

	DeltaInputsWeights.SetNum(InputsWeights.Num());
	DeltaHiddenWeights.SetNum(HiddenWeights.Num());
	ErrorGradientsHidden.SetNum(HiddenValues.Num());
	ErrorGradientsOutputs.SetNum(OutputsValues.Num());
}

void FNeuralNetwork::InitializeNetworkConfiguration(FNetworkConfiguration const& NetworkConfiguration)
{
	LearningRate = NetworkConfiguration.LearningRate;
	Momentum = NetworkConfiguration.Momentum;
	UseBatchLearning = NetworkConfiguration.UseBatchLearning;
	MaxEpochs = NetworkConfiguration.MaxEpochs;
}

void FNeuralNetwork::InitializeWeights()
{
	//RandRange for random, check others methods for better random
	for (uint32 InputIndex = 0; InputIndex <= NbInputs; InputIndex++)
	{
		for (uint32 HiddenIndex = 0; HiddenIndex <= NbHidden; HiddenIndex++)
		{
			uint32 const WeightIndex = GetInputWeightIndex(InputIndex, HiddenIndex);
			double const Weight = FMath::RandRange(-0.5, 0.5);
			InputsWeights[WeightIndex] = Weight;
		}
	}

	for (uint32 HiddenIndex = 0; HiddenIndex <= NbHidden; HiddenIndex++)
	{
		for (uint32 OutputIndex = 0; OutputIndex < NbOutputs; OutputIndex++)
		{
			uint32 const WeightIndex = GetHiddenWeightIndex(HiddenIndex, OutputIndex);
			double const Weight = FMath::RandRange(-0.5, 0.5);
			HiddenWeights[WeightIndex] = Weight;
		}
	}
}

void FNeuralNetwork::LoadWeights(TArray<double> const& Weight)
{
	for (uint32 InputIndex = 0; InputIndex < NbInputs * NbHidden; InputIndex++)
	{
		InputsWeights[InputIndex] = Weight[InputIndex];
	}

	for (uint32 HiddenIndex = 0; HiddenIndex < NbHidden * NbOutputs; HiddenIndex++)
	{
		InputsWeights[HiddenIndex] = Weight[HiddenIndex];
	}
}

TArray<double> const& FNeuralNetwork::Evaluate(TArray<double> const& Input)
{
	for (int32 i = 0; i < Input.Num(); i++)
	{
		InputsValues[i] = Input[i];
	}

	return Evaluate();
}

double FNeuralNetwork::GetHiddenErrorGradient(const uint32 HiddenIndex) const
{
	double ErrorsWeightedSum = 0;
	
	for (uint32 OutputIndex = 0; OutputIndex < NbOutputs; OutputIndex++)
	{
		uint32 const WeightIndex = GetHiddenWeightIndex(HiddenIndex, OutputIndex);
		ErrorsWeightedSum += HiddenWeights[WeightIndex] * ErrorGradientsOutputs[OutputIndex];
	}
	
	return HiddenValues[HiddenIndex] * (1.0 - HiddenValues[HiddenIndex]) * ErrorsWeightedSum;
}

void FNeuralNetwork::RunEpoch(FTrainingEntry const& TrainingEntry)
{
	Evaluate(TrainingEntry.Inputs);
	Backpropagation(TrainingEntry.ExpectedOutputs);
	UpdateWeights();
}

void FNeuralNetwork::Backpropagation(TArray<double> const& ExpectedOutputs)
{
	for (uint32 OutputIndex = 0; OutputIndex < NbOutputs; OutputIndex++)
	{
		ErrorGradientsOutputs[OutputIndex] =
			GetOutputErrorGradient(ExpectedOutputs[OutputIndex], OutputsValues[OutputIndex]);
		
		for (uint32 HiddenIndex = 0; HiddenIndex <= NbHidden; HiddenIndex++)
		{
			uint32 const WeightIndex = GetHiddenWeightIndex(HiddenIndex, OutputIndex);
			
			if (UseBatchLearning)
			{
				DeltaHiddenWeights[WeightIndex] += LearningRate * HiddenValues[HiddenIndex] *
					ErrorGradientsOutputs[OutputIndex];
			}
			else
			{
				DeltaHiddenWeights[WeightIndex] = LearningRate * HiddenValues[HiddenIndex] *
					ErrorGradientsOutputs[OutputIndex] + Momentum * DeltaHiddenWeights[WeightIndex];
			}
		}
	}

	for (uint32 HiddenIndex = 0; HiddenIndex < NbHidden; HiddenIndex++)
	{
		ErrorGradientsHidden[HiddenIndex] = GetHiddenErrorGradient(HiddenIndex);
		
		for (uint32 InputIndex = 0; InputIndex <= NbInputs; InputIndex++)
		{
			uint32 const WeightIndex = GetInputWeightIndex(InputIndex, HiddenIndex);
			
			if (UseBatchLearning)
			{
				DeltaInputsWeights[WeightIndex] += LearningRate * InputsValues[InputIndex] *
					ErrorGradientsHidden[InputIndex];
			}
			else
			{
				DeltaInputsWeights[WeightIndex] = LearningRate * InputsValues[InputIndex] *
					ErrorGradientsHidden[InputIndex] + Momentum * DeltaInputsWeights[WeightIndex];
			}
		}
	}

	if (!UseBatchLearning)
	{
		UpdateWeights();
	}
}

void FNeuralNetwork::UpdateWeights()
{
	for (uint32 InputIndex = 0; InputIndex <= NbInputs; InputIndex++)
	{
		for (uint32 HiddenIndex = 0; HiddenIndex <= NbHidden; HiddenIndex++)
		{
			uint32 const WeightIndex = GetInputWeightIndex(InputIndex, HiddenIndex);
			InputsWeights[WeightIndex] += DeltaInputsWeights[WeightIndex];
			
			if (UseBatchLearning)
			{
				DeltaInputsWeights[WeightIndex] = 0;
			}
		}
	}
	
	for (uint32 HiddenIndex = 0; HiddenIndex <= NbHidden; HiddenIndex++)
	{
		for (uint32 OutputIndex = 0; OutputIndex < NbOutputs; OutputIndex++)
		{
			uint32 const WeightIndex = GetHiddenWeightIndex(HiddenIndex, OutputIndex);
			HiddenWeights[WeightIndex] += DeltaHiddenWeights[WeightIndex];
			
			if (UseBatchLearning)
			{
				DeltaHiddenWeights[WeightIndex] = 0;
			}
		}
	}
}