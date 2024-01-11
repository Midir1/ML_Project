#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "InputActionValue.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Vehicle.generated.h"

UCLASS()
class ML_PROJECT_API AVehicle : public AWheeledVehiclePawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void SetThrottle(const FInputActionValue& Value);
	void SetBrake(const FInputActionValue& Value);
	void SetSteering(const FInputActionValue& Value);
	void SetHandbrake(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* ThrottleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* BrakeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SteeringAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* HandbrakeAction;
	
	UPROPERTY()
	UChaosVehicleMovementComponent* ChaosVehicleMovementComponent = nullptr;
};