#include "CubeMovement.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ACubeMovement::ACubeMovement()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACubeMovement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;

	if(Timer > TimeWanted)
	{
		Timer = 0.0f;

		//Update Entries first
		NetworkTick();
		//Train NN
		NeuralNetworkMain();
		//Update values with outputs
		NetworkUpdateValues();
	}
}

void ACubeMovement::BeginPlay()
{
	Super::BeginPlay();
	
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ACubeMovement::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ACubeMovement::NeuralNetworkMain()
{
	FNeuralNetwork::FNeuronsConfiguration NeuronsConfiguration;
	NeuronsConfiguration.NbInputs = NbInputs;
	NeuronsConfiguration.NbHidden = NbHidden;
	NeuronsConfiguration.NbOutputs = NbOutputs;
	
	FNeuralNetwork::FNetworkConfiguration NetworkConfiguration;
	NetworkConfiguration.LearningRate = LearningRate;
	NetworkConfiguration.Momentum = Momentum;
	NetworkConfiguration.UseBatchLearning = UseBatchLearning;
	NetworkConfiguration.MaxEpochs = MaxEpochs;
	NetworkConfiguration.DesiredAccuracy = DesiredAccuracy;

	const FNeuralNetwork NeuralNetwork(NeuronsConfiguration, NetworkConfiguration);
	ThisNeuralNetwork = NeuralNetwork;

	ThisNeuralNetwork.Train(Data);
}

void ACubeMovement::NetworkTick()
{
	//First doing with only horizontal axis : Y axis
	//const FVector Distance = Sphere->GetActorLocation() - GetActorLocation();
	const float Distance = Sphere->GetActorLocation().Y - GetActorLocation().Y;

	//Condition
	if(Distance != 0.0f)
	{
		Entries.push_back(FNeuralNetwork::FTrainingEntry());

		uint32 i = 0;
		
		while (i < NbInputs + NbOutputs)
		{
			if(Entries.back().Inputs.size() < NbInputs)
			{
				Entries.back().Inputs.push_back(Distance);
			}
			else
			{
				//Right : D
				if(Distance > 0.0f)
				{
					Entries.back().ExpectedOutputs.push_back(1.0f);
				}
				//Left : Q
				else
				{
					Entries.back().ExpectedOutputs.push_back(-1.0f);
				}
			}

			i++;
		}
		
		if(!Entries.empty())
		{
			// Temp WIP
			// Training set
			int32 const numEntries = Entries.size();
			int32 const numTrainingEntries  = static_cast<int32>(0.6 * numEntries);
			int32 const numGeneralizationEntries = static_cast<int32>(ceil(0.2 * numEntries));
			
			for (int32 j = 0; j < numTrainingEntries; j++)
			{
				Data.TrainingSet.push_back(Entries[j]);
			}

			// Generalization set
			for (int32 x = 0 ; x < numTrainingEntries + numGeneralizationEntries; x++)
			{
				Data.GeneralizationSet.push_back(Entries[x]);
			}

			// Validation set
			for (int32 z = 0 ; z < numEntries; z++)
			{
				Data.ValidationSet.push_back(Entries[z]);
			}
		}
	}
}

void ACubeMovement::NetworkUpdateValues()
{
	const float Distance = Sphere->GetActorLocation().Y - GetActorLocation().Y;
	
	// Not here for changing movement value
	// ThisNeuralNetwork is changed in NeuralNetworkMain
	//Must check outputsValuesClamped because always returning 0
	if(ThisNeuralNetwork.GetNbOutputs() > 0 && Controller != nullptr)
	{
		if (Controller != nullptr)
		{
			//AddMovementInput(GetActorForwardVector(), MovementVector.Y);
			AddMovementInput(GetActorRightVector(), ThisNeuralNetwork.GetOutputsValuesClamped());
		}
	
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow,
			FString::Printf(TEXT("Distance, Output : %f, %d"), Distance,
				ThisNeuralNetwork.GetOutputsValuesClamped()));
	}
}

void ACubeMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACubeMovement::Move);
	}
}