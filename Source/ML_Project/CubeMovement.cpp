#include "CubeMovement.h"

#include "NeuralNetworkDataWidget.h"

ACubeMovement::ACubeMovement()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACubeMovement::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeNeuralNetwork();
	Timer = MovementTimer;
}

void ACubeMovement::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!NeuralNetwork.IsTrainingOver())
	{
		Timer += DeltaTime;

		if(Timer > MovementTimer)
		{
			Timer = 0.0f;
			
			// EntriesTick();
			// NeuralNetwork.Train(Entry);
		}
		
		EntriesTick();
		NeuralNetwork.Train(Entry);
		OutputsValuesTick(false);
	}
	else
	{
		NeuralNetwork.Evaluate();
		OutputsValuesTick(true);
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

//TODO : Check Output Value To Make Cube Move With Two Axis
void ACubeMovement::OutputsValuesTick(const bool TrainingOver)
{
	const float Distance = Sphere->GetActorLocation().Y - GetActorLocation().Y;
	
	if(NeuralNetwork.GetNbOutputs() > 0 && Controller != nullptr)
	{
		float const MoveValue = 2.0f * static_cast<float>(NeuralNetwork.GetOutputsValuesClamped()) - 1.0f;
		
		AddMovementInput(GetActorRightVector(), MoveValue);
		NeuralNetworkDataWidget->SetWidgetData(Distance, MoveValue, TrainingOver);
	}
}