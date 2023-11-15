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

	InitializeNeuralNetwork();
}

void ACubeMovement::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;

	if(Timer > TimeWanted)
	{
		Timer = 0.0f;

		EntriesTick();
		// NeuralNetwork.Train(Data);
		// OutputsValuesTick();
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

void ACubeMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACubeMovement::Move);
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
		Entries.push_back(FTrainingEntry());

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
					Entries.back().ExpectedOutputs.push_back(1.0);
				}
				//Left : Q
				else
				{
					Entries.back().ExpectedOutputs.push_back(-1.0);
				}
			}

			i++;
		}
		
		if(!Entries.empty())
		{
			size_t const NbEntries = Entries.size();
			size_t const NbTrainingEntries = 0.6f * NbEntries;
			size_t const NbGeneralizationEntries = ceil(0.2f * NbEntries);
			
			for (size_t x = 0; x < NbTrainingEntries; x++)
			{
				Data.TrainingSet.push_back(Entries[x]);
			}
			
			for (size_t y = 0 ; y < NbTrainingEntries + NbGeneralizationEntries; y++)
			{
				Data.GeneralizationSet.push_back(Entries[y]);
			}
			
			for (size_t z = 0 ; z < NbEntries; z++)
			{
				Data.ValidationSet.push_back(Entries[z]);
			}
		}
	}
}

void ACubeMovement::OutputsValuesTick()
{
	const float Distance = Sphere->GetActorLocation().Y - GetActorLocation().Y;
	
	if(NeuralNetwork.GetNbOutputs() > 0 && Controller != nullptr)
	{
		if (Controller != nullptr)
		{
			//AddMovementInput(GetActorForwardVector(), MovementVector.Y);
			AddMovementInput(GetActorRightVector(), NeuralNetwork.GetOutputsValuesClamped());
		}
	
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow,
			FString::Printf(TEXT("Distance, Output : %f, %d"), Distance,
				NeuralNetwork.GetOutputsValuesClamped()));
	}
}