#include "Vehicle.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "LearningAgentsManager.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void AVehicle::BeginPlay()
{
	Super::BeginPlay();

	ChaosVehicleMovementComponent = FindComponentByClass<UChaosWheeledVehicleMovementComponent>();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Register Agents With Manager
	ReceiveBeginPlay();

	AActor* ManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), ALearningAgentsManager::StaticClass());

	if(ManagerActor)
	{
		AgentID = Cast<ALearningAgentsManager>(ManagerActor)->AddAgent(this);
	}
}

void AVehicle::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AVehicle::SetThrottle);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &AVehicle::SetBrake);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &AVehicle::SetSteering);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Triggered, this, &AVehicle::SetHandbrake);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &AVehicle::SetHandbrake);
	}
}

void AVehicle::SetThrottle(const FInputActionValue& Value)
{
	const float ThrottleValue = Value.Get<float>();

	if (ChaosVehicleMovementComponent)
	{
		ChaosVehicleMovementComponent->SetThrottleInput(ThrottleValue);
	}
}

void AVehicle::SetBrake(const FInputActionValue& Value)
{
	const float BrakeValue = Value.Get<float>();
	
	if (ChaosVehicleMovementComponent)
	{
		ChaosVehicleMovementComponent->SetBrakeInput(BrakeValue);
	}
}

void AVehicle::SetSteering(const FInputActionValue& Value)
{
	const float SteeringValue = Value.Get<float>();
	
	if (ChaosVehicleMovementComponent)
	{
		ChaosVehicleMovementComponent->SetSteeringInput(SteeringValue);
	}
}

void AVehicle::SetHandbrake(const FInputActionValue& Value)
{
	const bool DoHandbrake = Value.Get<bool>();
	
	if (ChaosVehicleMovementComponent)
	{
		ChaosVehicleMovementComponent->SetHandbrakeInput(DoHandbrake);
	}
}