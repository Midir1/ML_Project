#include "CubeMovement.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ACubeMovement::ACubeMovement()
{
	PrimaryActorTick.bCanEverTick = true;
	
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

	NeuralNetworkMain();
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

void ACubeMovement::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	//First doing with only horizontal axis : Y axis
	//const FVector Distance = Sphere->GetActorLocation() - GetActorLocation();
	const float Distance = Sphere->GetActorLocation().Y - GetActorLocation().Y;

	//Condition
	if(Distance != 0.0f)
	{
		Entries.push_back(FNeuralNetwork::FTrainingEntry());
		FNeuralNetwork::FTrainingEntry& TrainingEntries = Entries.back();
		
		TrainingEntries.Inputs.push_back(Distance);

		//Right : D
		if(Distance > 0.0f)
		{
			TrainingEntries.ExpectedOutputs.push_back(1.0f);
		}
		//Left : Q
		else
		{
			TrainingEntries.ExpectedOutputs.push_back(-1.0f);
		}
	}

	// Temp WIP
	// Training set
	int32 const numEntries = Entries.size();
	int32 const numTrainingEntries  = static_cast<int32>(0.6 * numEntries);
	int32 const numGeneralizationEntries = static_cast<int32>(ceil(0.2 * numEntries));

	int32 entryIdx = 0;
	for ( ; entryIdx < numTrainingEntries; entryIdx++ )
	{
		Data.TrainingSet.push_back( Entries[entryIdx] );
	}

	// Generalization set
	for ( ; entryIdx < numTrainingEntries + numGeneralizationEntries; entryIdx++ )
	{
		Data.GeneralizationSet.push_back( Entries[entryIdx] );
	}

	// Validation set
	for ( ; entryIdx < numEntries; entryIdx++ )
	{
		Data.ValidationSet.push_back( Entries[entryIdx] );
	}

	//Must check outputsValuesClamped because always returning 0
	if(ThisNeuralNetwork.GetNbOutputs() > 0)
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