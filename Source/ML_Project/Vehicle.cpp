#include "Vehicle.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
}

void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AVehicle::SetThrottle);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &AVehicle::SetBrake);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &AVehicle::SetSteering);
		//Pressed
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Triggered, this, &AVehicle::SetHandbrake);
		//IE_Released
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Triggered, this, &AVehicle::SetHandbrake);
	}
}

void AVehicle::SetThrottle(const FInputActionValue& Value)
{
	//ThrottleValue = FMath::Clamp(ThrottleValue, -1.0f, 1.0f);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));	
	
	if (ChaosVehicleMovementComponent)
	{
		//ChaosVehicleMovementComponent->SetThrottleInput(ThrottleValue);
	}
}

void AVehicle::SetBrake(const FInputActionValue& Value)
{
	//BrakeValue = FMath::Clamp(BrakeValue, -1.0f, 1.0f);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));

	// Set the throttle input for the Chaos vehicle
	if (ChaosVehicleMovementComponent)
	{
		//ChaosVehicleMovementComponent->SetBrakeInput(BrakeValue);
	}
}

void AVehicle::SetSteering(const FInputActionValue& Value)
{
	// Ensure the ThrottleValue is clamped between -1.0 and 1.0
	//SteeringValue = FMath::Clamp(SteeringValue, -1.0f, 1.0f);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));

	// Set the throttle input for the Chaos vehicle
	if (ChaosVehicleMovementComponent)
	{
		//ChaosVehicleMovementComponent->SetSteeringInput(SteeringValue);
	}
}

void AVehicle::SetHandbrake(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
	
	// Set the throttle input for the Chaos vehicle
	if (ChaosVehicleMovementComponent)
	{
		ChaosVehicleMovementComponent->SetHandbrakeInput(true);
	}
}