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
	ANeuralNetwork::FNeuronsConfiguration NeuronsConfiguration;
	NeuronsConfiguration.NbInputs = NbInputs;
	NeuronsConfiguration.NbHidden = NbHidden;
	NeuronsConfiguration.NbOutputs = NbOutputs;
	
	ANeuralNetwork::FNetworkConfiguration NetworkConfiguration;
	NetworkConfiguration.LearningRate = LearningRate;
	NetworkConfiguration.Momentum = Momentum;
	NetworkConfiguration.UseBatchLearning = UseBatchLearning;
	NetworkConfiguration.MaxEpochs = MaxEpochs;
	NetworkConfiguration.DesiredAccuracy = DesiredAccuracy;

	ANeuralNetwork NeuralNetwork(NeuronsConfiguration, NetworkConfiguration);

	//NeuralNetwork.Train();
}

void ACubeMovement::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Condition
	if(GetActorLocation() != Sphere->GetActorLocation())
	{
		Entries.push_back(ANeuralNetwork::FTrainingEntry());
		ANeuralNetwork::FTrainingEntry& TrainingEntries = Entries.back();
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