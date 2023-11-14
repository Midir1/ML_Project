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
}

void ACubeMovement::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;

	if(Timer > TimeWanted)
	{
		Timer = 0.0f;

		EntriesTick();
		TrainNeuralNetworkTick();
		OutputsValuesTick();
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
			// TODO : Not good values
			auto NumEntries = Entries.size();
			auto NumTrainingEntries = (NumEntries == 1) ? 1 : 0.6f * NumEntries;
			auto NumGeneralizationEntries = ceil(0.2f * NumEntries);
			
			for (int32 x = 0; x < NumTrainingEntries; x++)
			{
				Data.TrainingSet.push_back(Entries[x]);
			}
			
			for (int32 y = 0 ; y < NumTrainingEntries + NumGeneralizationEntries; y++)
			{
				Data.GeneralizationSet.push_back(Entries[y]);
			}
			
			for (int32 z = 0 ; z < NumEntries; z++)
			{
				Data.ValidationSet.push_back(Entries[z]);
			}
		}
	}
}

void ACubeMovement::TrainNeuralNetworkTick()
{
	FNeuronsConfiguration NeuronsConfiguration;
	NeuronsConfiguration.NbInputs = NbInputs;
	NeuronsConfiguration.NbHidden = NbHidden;
	NeuronsConfiguration.NbOutputs = NbOutputs;
	
	FNetworkConfiguration NetworkConfiguration;
	NetworkConfiguration.LearningRate = LearningRate;
	NetworkConfiguration.Momentum = Momentum;
	NetworkConfiguration.UseBatchLearning = UseBatchLearning;
	NetworkConfiguration.MaxEpochs = MaxEpochs;
	NetworkConfiguration.DesiredAccuracy = DesiredAccuracy;
	
	NeuralNetwork = FNeuralNetwork(NeuronsConfiguration, NetworkConfiguration);
	NeuralNetwork.Train(Data);
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