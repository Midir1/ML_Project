#include "Agent.h"
#include "NeuralNetworkDataWidget.h"
#include "NeuralNetworkJson.h"

AAgent::AAgent()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAgent::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeNeuralNetwork();
	GetWorldSettings()->SetTimeDilation(TimeDilation);
}

void AAgent::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	EntriesTick();
	NeuralNetwork.Train(Entry);

	if(!NeuralNetwork.IsTrainingDone())
	{
		OutputsValuesTick(false);
	}
	else
	{
		OutputsValuesTick(true);
		SaveNeuronsDataToJson();
	}
}

void AAgent::InitializeNeuralNetwork()
{
	NeuronsConfiguration.NbInputs = NbInputs;
	NeuronsConfiguration.NbHidden = NbHidden;
	NeuronsConfiguration.NbOutputs = NbOutputs;
	
	NetworkConfiguration.LearningRate = LearningRate;
	NetworkConfiguration.Momentum = Momentum;
	NetworkConfiguration.MaxEpochs = MaxEpochs;

	const uint32 TotalNeurons = NbInputs + NbHidden + NbOutputs;
	TArray<uint32> NbNeurons;
	TArray<double> Weights;

	if(FNeuralNetworkJson::DeserializeJson(NbNeurons, Weights)
		&& TotalNeurons == NbNeurons[0] + NbNeurons[1] + NbNeurons[2])
	{
		NeuralNetwork = FNeuralNetwork(NeuronsConfiguration, NetworkConfiguration, Weights);
	}
	else
	{
		NeuralNetwork = FNeuralNetwork(NeuronsConfiguration, NetworkConfiguration);
	}
	
}

//TODO : Making "Raycasts" to give vision instead of relying on distance
void AAgent::EntriesTick()
{
	const FVector Distance = Sphere->GetActorLocation() - GetActorLocation();
	
	if(Distance != FVector::Zero())
	{
		Entry = FTrainingEntry();
		
		uint32 i = 0;
		
		while (i < 2)
		{
			if(static_cast<uint32>(Entry.Inputs.Num()) < NbInputs)
			{
				Entry.Inputs.Add(Distance.Y);
				Entry.Inputs.Add(Distance.X);
			}
			else
			{
				//Right : D
				if(Distance.Y > 50.0f)
				{
					Entry.ExpectedOutputs.Add(1.0);
				}
				//Left : Q
				else if(Distance.Y < -50.0f)
				{
					Entry.ExpectedOutputs.Add(0.0);
				}
				else
				{
					Entry.ExpectedOutputs.Add(0.5);
				}

				//Up : Z
				if(Distance.X > 50.0f)
				{
					Entry.ExpectedOutputs.Add(1.0);
				}
				//Down : S
				else if(Distance.X < -50.0f)
				{
					Entry.ExpectedOutputs.Add(0.0);
				}
				else
				{
					Entry.ExpectedOutputs.Add(0.5);
				}
			}

			i++;
		}
	}
}

void AAgent::OutputsValuesTick(const bool TrainingOver)
{
	const float Distance = Sphere->GetActorLocation().Y - GetActorLocation().Y;
	
	if(NeuralNetwork.GetNbOutputs() > 0 && Controller != nullptr)
	{
		float const MoveValueX = 2.0f * static_cast<float>(NeuralNetwork.GetOutputsValuesClamped()[0]) - 1.0f;
		float const MoveValueY = 2.0f * static_cast<float>(NeuralNetwork.GetOutputsValuesClamped()[1]) - 1.0f;
		
		AddMovementInput(GetActorRightVector(), MoveValueX);
		AddMovementInput(GetActorForwardVector(), MoveValueY);
		
		NeuralNetworkDataWidget->SetWidgetData(Distance, MoveValueX, MoveValueY, TrainingOver);
	}
}

void AAgent::SaveNeuronsDataToJson()
{
	if(!JsonSaved)
	{
		JsonSaved = true;
		FNeuralNetworkJson::SerializeNbNeuronsToJson(NeuralNetwork);
		FNeuralNetworkJson::SerializeNeuronsWeightsToJson(NeuralNetwork);
	}
}