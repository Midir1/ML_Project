#include "CubeMovement.h"
#include "NeuralNetworkDataWidget.h"
#include "NeuralNetworkJson.h"

ACubeMovement::ACubeMovement()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACubeMovement::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeNeuralNetwork();
	GetWorldSettings()->SetTimeDilation(TimeDilation);
}

void ACubeMovement::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!NeuralNetwork.IsTrainingOver())
	{
		EntriesTick();
		NeuralNetwork.Train(Entry);
		OutputsValuesTick(false);
	}
	else
	{
		NeuralNetwork.Evaluate();
		OutputsValuesTick(true);

		//Every Frame change Function Json location
		if(!JsonSaved)
		{
			JsonSaved = true;
			FNeuralNetworkJson::SerializeToJson(NeuralNetwork);
		}
	}
}

void ACubeMovement::InitializeNeuralNetwork()
{
	NeuronsConfiguration.NbInputs = NbInputs;
	NeuronsConfiguration.NbHidden = NbHidden;
	NeuronsConfiguration.NbOutputs = NbOutputs;
	
	NetworkConfiguration.LearningRate = LearningRate;
	NetworkConfiguration.Momentum = Momentum;
	NetworkConfiguration.UseBatchLearning = UseBatchLearning;
	NetworkConfiguration.MaxEpochs = MaxEpochs;

	if(FNeuralNetworkJson::DeserializeJsonToStruct())
	{
		
	}
	
	NeuralNetwork = FNeuralNetwork(NeuronsConfiguration, NetworkConfiguration);
}

void ACubeMovement::EntriesTick()
{
	//First doing with only horizontal axis : Y axis
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
				if(Distance.Y > 0.0f)
				{
					Entry.ExpectedOutputs.Add(1.0);
				}
				//Left : Q
				else
				{
					Entry.ExpectedOutputs.Add(0.0);
				}

				//Up : Z
				if(Distance.X > 0.0f)
				{
					Entry.ExpectedOutputs.Add(1.0);
				}
				//Down : S
				else
				{
					Entry.ExpectedOutputs.Add(0.0);
				}
			}

			i++;
		}
	}
}

void ACubeMovement::OutputsValuesTick(const bool TrainingOver)
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