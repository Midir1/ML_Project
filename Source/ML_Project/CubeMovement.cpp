#include "CubeMovement.h"

ACubeMovement::ACubeMovement()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACubeMovement::BeginPlay()
{
	Super::BeginPlay();
	
	

	InitializeNeuralNetwork();
}

void ACubeMovement::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!NeuralNetwork.IsTrainingOver())
	{
		EntriesTick();
		NeuralNetwork.Train(Data);
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
	NetworkConfiguration.DesiredAccuracy = DesiredAccuracy;
	
	NeuralNetwork = FNeuralNetwork(NeuronsConfiguration, NetworkConfiguration);
}

void ACubeMovement::EntriesTick()
{
	//First doing with only horizontal axis : Y axis
	//const FVector Distance = Sphere->GetActorLocation() - GetActorLocation();
	const float Distance = Sphere->GetActorLocation().Y - GetActorLocation().Y;
	
	if(Distance != 0.0f)
	{
		Entries.Add(FTrainingEntry());

		uint32 i = 0;
		
		while (i < NbInputs + NbOutputs)
		{
			if(static_cast<uint32>(Entries.Last().Inputs.Num()) < NbInputs)
			{
				Entries.Last().Inputs.Add(Distance);
			}
			else
			{
				//Right : D
				if(Distance > 0.0f)
				{
					Entries.Last().ExpectedOutputs.Add(1.0);
				}
				//Left : Q
				else
				{
					Entries.Last().ExpectedOutputs.Add(0.0);
				}
			}

			i++;
		}
		
		if(Entries.Num() > 0)
		{
			size_t const NbEntries = Entries.Num();
			size_t const NbTrainingEntries = 0.6f * NbEntries;
			size_t const NbGeneralizationEntries = ceil(0.2f * NbEntries);
			
			for (size_t x = 0; x < NbTrainingEntries; x++)
			{
				Data.TrainingSet.Add(Entries[x]);
			}
			
			for (size_t y = 0 ; y < NbTrainingEntries + NbGeneralizationEntries; y++)
			{
				Data.GeneralizationSet.Add(Entries[y]);
			}
			
			for (size_t z = 0 ; z < NbEntries; z++)
			{
				Data.ValidationSet.Add(Entries[z]);
			}
		}
	}
}

void ACubeMovement::OutputsValuesTick(bool TrainingOver)
{
	const float Distance = Sphere->GetActorLocation().Y - GetActorLocation().Y;
	
	if(NeuralNetwork.GetNbOutputs() > 0 && Controller != nullptr)
	{
		float const MoveValue = 2.0f * static_cast<float>(NeuralNetwork.GetOutputsValuesClamped()) - 1.0f;
		
		AddMovementInput(GetActorRightVector(), MoveValue);

		if(!TrainingOver)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow,
			FString::Printf(TEXT("Distance, Output : %f, %f"), Distance, MoveValue));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green,
			FString::Printf(TEXT("Distance, Output : %f, %f"), Distance, MoveValue));
		}
	}
}