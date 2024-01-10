#include "Vehicle.h"

void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("Throttle", this, &AVehicle::SetThrottle);
	PlayerInputComponent->BindAxis("Brake", this, &AVehicle::SetBrake);
	PlayerInputComponent->BindAxis("Steering", this, &AVehicle::SetSteering);
	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &AVehicle::SetHandbrake);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AVehicle::UnsetHandbrake);
}

void AVehicle::SetThrottle(float ThrottleValue)
{
	ThrottleValue = FMath::Clamp(ThrottleValue, -1.0f, 1.0f);
	
	if (ChaosVehicleMovementComponent)
	{
		ChaosVehicleMovementComponent->SetThrottleInput(ThrottleValue);
	}
}

void AVehicle::SetBrake(float BrakeValue)
{
	BrakeValue = FMath::Clamp(BrakeValue, -1.0f, 1.0f);

	// Set the throttle input for the Chaos vehicle
	if (ChaosVehicleMovementComponent)
	{
		ChaosVehicleMovementComponent->SetBrakeInput(BrakeValue);
	}
}

void AVehicle::SetSteering(float SteeringValue)
{
	// Ensure the ThrottleValue is clamped between -1.0 and 1.0
	SteeringValue = FMath::Clamp(SteeringValue, -1.0f, 1.0f);

	// Set the throttle input for the Chaos vehicle
	if (ChaosVehicleMovementComponent)
	{
		ChaosVehicleMovementComponent->SetSteeringInput(SteeringValue);
	}
}

void AVehicle::SetHandbrake()
{
	// Set the throttle input for the Chaos vehicle
	if (ChaosVehicleMovementComponent)
	{
		ChaosVehicleMovementComponent->SetHandbrakeInput(true);
	}
}

void AVehicle::UnsetHandbrake()
{
	// Set the throttle input for the Chaos vehicle
	if (ChaosVehicleMovementComponent)
	{
		ChaosVehicleMovementComponent->SetHandbrakeInput(false);
	}
}

