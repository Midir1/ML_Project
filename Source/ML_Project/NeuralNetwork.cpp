#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork()
{
	InitializeNbNeurons();
	InitializeNetwork();
	InitializeWeights();
}

NeuralNetwork::NeuralNetwork(std::vector<double> const& Weights)
{
	InitializeNbNeurons();
	InitializeNetwork();
	LoadWeights(Weights);
}

void NeuralNetwork::Train(FTrainingData const& TrainingData)
{
	// Reset
	CurrentEpoch = 0;
	TrainingSetAccuracy = 0;
	ValidationSetAccuracy = 0;
	GeneralizationSetAccuracy = 0;
	TrainingSetAccuracy = 0;
	ValidationSetMse = 0;
	GeneralizationSetMse = 0;

	while((TrainingSetAccuracy < DesiredAccuracy || GeneralizationSetAccuracy < DesiredAccuracy)
		&& CurrentEpoch < MaxEpochs)
	{
		RunEpoch(TrainingData.TrainingSet);
		GetSetAccuracyAndMse(TrainingData.GeneralizationSet, GeneralizationSetAccuracy, GeneralizationSetMse);
		CurrentEpoch++;
	}

	GetSetAccuracyAndMse(TrainingData.ValidationSet, ValidationSetAccuracy, ValidationSetMse);
}

void NeuralNetwork::InitializeNbNeurons()
{
	NbInputs = NeuronsConfiguration.NbInputs;
	NbHidden = NeuronsConfiguration.NbHidden;
	NbOutputs = NeuronsConfiguration.NbOutputs;
}

void NeuralNetwork::InitializeNetwork()
{
	//Resizing the neurons values of InputLayer and HiddenLayer to add bias neurons
	InputsValues.resize(NbInputs + 1);
	HiddenValues.resize(NbHidden + 1);
	OutputsValues.resize(NbOutputs);
	OutputsValuesClamped.resize(NbOutputs);

	//Initialization of neurons values (fill instead of memset)
	std::fill(InputsValues.begin(), InputsValues.end(), 0);
	std::fill(HiddenValues.begin(), HiddenValues.end(), 0);
	std::fill(OutputsValues.begin(), OutputsValues.end(), 0);
	std::fill(OutputsValuesClamped.begin(), OutputsValuesClamped.end(), 0);

	//Set bias values
	InputsValues.back() = -1.0;
	HiddenValues.back() = -1.0;

	//Initialization of neurons weights
	InputsWeights.resize((NbInputs + 1) * (NbHidden + 1));
	HiddenWeights.resize((NbHidden + 1) * (NbOutputs + 1));

	DeltaInputsWeights.resize(InputsWeights.size());
	DeltaHiddenWeights.resize(HiddenWeights.size());
	ErrorGradientsHidden.resize(HiddenValues.size());
	ErrorGradientsOutputs.resize(OutputsValues.size());

	//Initialization of delta neurons values and for error gradients (fill instead of memset)
	std::fill(DeltaInputsWeights.begin(), DeltaInputsWeights.end(), 0);
	std::fill(DeltaHiddenWeights.begin(), DeltaHiddenWeights.end(), 0);
	std::fill(ErrorGradientsHidden.begin(), ErrorGradientsHidden.end(), 0);
	std::fill(ErrorGradientsOutputs.begin(), ErrorGradientsOutputs.end(), 0);
}

void NeuralNetwork::InitializeWeights()
{
	for (uint32 InputIndex = 0; InputIndex <= NbInputs; InputIndex++)
	{
		for (uint32 HiddenIndex = 0; HiddenIndex <= NbHidden; HiddenIndex++)
		{
			uint32 const WeightIndex = GetInputWeightIndex(InputIndex, HiddenIndex);
			// double const Weight;
			// InputsWeights[WeightIndex] = Weight;
		}
	}

	for (uint32 HiddenIndex = 0; HiddenIndex <= NbHidden; HiddenIndex++)
	{
		for (uint32 OutputIndex = 0; OutputIndex <= NbOutputs; OutputIndex++)
		{
			uint32 const WeightIndex = GetHiddenWeightIndex(HiddenIndex, OutputIndex);
			// double const Weight;
			// HiddenWeights[WeightIndex] = Weight;
		}
	}
}

void NeuralNetwork::LoadWeights(std::vector<double> const& Weight)
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

std::vector<uint32> const& NeuralNetwork::Evaluate(std::vector<double> const& Input)
{
	memcpy(InputsValues.data(), Input.data(), Input.size() * sizeof(double));

	for (uint32 HiddenIndex = 0; HiddenIndex < NbHidden; HiddenIndex++)
	{
		HiddenValues[HiddenIndex] = 0;
		
		for (uint32 InputIndex = 0; InputIndex < NbInputs; InputIndex++)
		{
			uint32 const WeightIndex = GetInputWeightIndex(InputIndex, HiddenIndex);
			HiddenValues[HiddenIndex] += InputsValues[InputIndex] * InputsWeights[WeightIndex];
		}

		HiddenValues[HiddenIndex] = Sigmoid(HiddenValues[HiddenIndex]);
	}

	for (uint32 OutputIndex = 0; OutputIndex < NbOutputs; OutputIndex++)
	{
		OutputsValues[OutputIndex] = 0;
		
		for (uint32 HiddenIndex = 0; HiddenIndex < NbHidden; HiddenIndex++)
		{
			uint32 const WeightIndex = GetHiddenWeightIndex(HiddenIndex, OutputIndex);
			OutputsValues[OutputIndex] += HiddenValues[HiddenIndex] * HiddenWeights[WeightIndex];
		}

		OutputsValues[OutputIndex] = Sigmoid(OutputsValues[OutputIndex]);
		OutputsValuesClamped[OutputIndex] = ClampOutputValue(OutputsValues[OutputIndex]);
	}

	return OutputsValuesClamped;
}

double NeuralNetwork::GetHiddenErrorGradient(const uint32 HiddenIndex) const
{
	double ErrorsWeightedSum = 0;
	
	for (uint32 OutputIndex = 0; OutputIndex < NbOutputs; OutputIndex++)
	{
		uint32 const WeightIndex = GetHiddenWeightIndex(HiddenIndex, OutputIndex);
		ErrorsWeightedSum += HiddenWeights[WeightIndex] * ErrorGradientsOutputs[OutputIndex];
	}
	
	return HiddenValues[HiddenIndex] * (1.0 - HiddenValues[HiddenIndex]) * ErrorsWeightedSum;
}

void NeuralNetwork::RunEpoch(FTrainingSet const& TrainingSet)
{
	double IncorrectEntries = 0;
	double Mse = 0;

	for (FTrainingEntry const& TrainingEntry : TrainingSet)
	{
		Evaluate(TrainingEntry.Inputs);
		Backpropagation(TrainingEntry.ExpectedOutputs);
		
		bool ResultCorrect = true;
		
		for (uint32 OutputIndex = 0; OutputIndex < NbOutputs; OutputIndex++)
		{
			if (OutputsValuesClamped[OutputIndex] != TrainingEntry.ExpectedOutputs[OutputIndex])
			{
				ResultCorrect = false;
			}
			
			Mse += pow((OutputsValues[OutputIndex] - TrainingEntry.ExpectedOutputs[OutputIndex]), 2);
		}

		if (!ResultCorrect)
		{
			IncorrectEntries++;
		}
	}
	
	if (UseBatchLearning)
	{
		UpdateWeights();
	}
	
	TrainingSetAccuracy = 100.0 - (IncorrectEntries / TrainingSet.size() * 100.0);
	TrainingSetMse = Mse / (NbOutputs * TrainingSet.size());
}

void NeuralNetwork::Backpropagation(std::vector<uint32> const& ExpectedOutputs)
{
	for (uint32 OutputIndex = 0; OutputIndex < NbOutputs; OutputIndex++)
	{
		ErrorGradientsOutputs[OutputIndex] =
			GetOutputErrorGradient(ExpectedOutputs[OutputIndex], OutputsValues[OutputIndex]);
		
		for (uint32 HiddenIndex = 0; HiddenIndex <= NbHidden; HiddenIndex++)
		{
			int32_t const WeightIndex = GetHiddenWeightIndex(HiddenIndex, OutputIndex);
			
			if (UseBatchLearning)
			{
				DeltaHiddenWeights[WeightIndex] += LearningRate * HiddenValues[HiddenIndex] *
					ErrorGradientsOutputs[OutputIndex];
			}
			else
			{
				DeltaHiddenWeights[WeightIndex] = LearningRate * HiddenValues[HiddenIndex] *
					ErrorGradientsOutputs[OutputIndex]+ Momentum * DeltaHiddenWeights[WeightIndex];
			}
		}
	}

	for (uint32 HiddenIndex = 0; HiddenIndex < NbHidden; HiddenIndex++)
	{
		ErrorGradientsHidden[HiddenIndex] = GetHiddenErrorGradient(HiddenIndex);
		
		for (uint32 InputIndex = 0; InputIndex <= NbInputs; InputIndex++)
		{
			int32_t const WeightIndex = GetInputWeightIndex(InputIndex, HiddenIndex);
			
			if (UseBatchLearning)
			{
				DeltaInputsWeights[WeightIndex] += LearningRate * InputsValues[InputIndex] *
					ErrorGradientsHidden[InputIndex];
			}
			else
			{
				DeltaInputsWeights[WeightIndex] = LearningRate * InputsValues[InputIndex] *
					ErrorGradientsHidden[InputIndex]+ Momentum * DeltaInputsWeights[WeightIndex];
			}
		}
	}

	if (!UseBatchLearning)
	{
		UpdateWeights();
	}
}

void NeuralNetwork::UpdateWeights()
{
	for (uint32 InputIndex = 0; InputIndex <= NbInputs; InputIndex++)
	{
		for (uint32 HiddenIndex = 0; HiddenIndex <= NbHidden; HiddenIndex++)
		{
			int32_t const WeightIndex = GetInputWeightIndex(InputIndex, HiddenIndex);
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
			int32_t const WeightIndex = GetHiddenWeightIndex(HiddenIndex, OutputIndex);
			HiddenWeights[WeightIndex] += DeltaHiddenWeights[WeightIndex];
			
			if (UseBatchLearning)
			{
				DeltaHiddenWeights[WeightIndex] = 0;
			}
		}
	}
}

void NeuralNetwork::GetSetAccuracyAndMse(FTrainingSet const& TrainingSet, double& Accuracy, double& Mse)
{
	Accuracy = 0;
	Mse = 0;

	double NbIncorrectResults = 0;
	
	for (FTrainingEntry const& TrainingEntry : TrainingSet)
	{
		Evaluate(TrainingEntry.Inputs);
		
		bool CorrectResult = true;
		
		for (uint32 OutputIndex = 0; OutputIndex < NbOutputs; OutputIndex++)
		{
			if (OutputsValuesClamped[OutputIndex] != TrainingEntry.ExpectedOutputs[OutputIndex])
			{
				CorrectResult = false;
			}

			Mse += pow((OutputsValuesClamped[OutputIndex] - TrainingEntry.ExpectedOutputs[OutputIndex]), 2);
		}

		if (!CorrectResult)
		{
			NbIncorrectResults++;
		}
	}

	Accuracy = 100.0f - ( NbIncorrectResults / TrainingSet.size() * 100.0);
	Mse = Mse / (NbOutputs * TrainingSet.size());
}
