#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Vehicle.generated.h"

UCLASS()
class ML_PROJECT_API AVehicle : public AWheeledVehiclePawn
{
	GENERATED_BODY()
	

	

private:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void SetThrottle(float ThrottleValue);
	void SetBrake(float BrakeValue);
	void SetSteering(float SteeringValue);
	void SetHandbrake();
	void UnsetHandbrake();

	//TODO : Check I Can Find This In Inspector
	UPROPERTY(EditAnywhere)
	UChaosVehicleMovementComponent* ChaosVehicleMovementComponent = nullptr;
};