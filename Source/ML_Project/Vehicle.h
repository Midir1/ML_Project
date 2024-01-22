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
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void SetThrottle(const FInputActionValue& Value);
	void SetBrake(const FInputActionValue& Value);
	void SetSteering(const FInputActionValue& Value);
	void SetHandbrake(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* ThrottleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* BrakeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SteeringAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* HandbrakeAction;
	
	UPROPERTY()
	UChaosVehicleMovementComponent* ChaosVehicleMovementComponent = nullptr;

	// TODO : Doing Q-Learning Implementation
	
	// UPROPERTY(EditAnywhere)
	// int32 NumStates = 6;
	//
	// UPROPERTY(EditAnywhere)
	// int32 NumActions = 2;
	//
	// UPROPERTY(EditAnywhere)
	// float LearningRate = 0.8;
	//
	// UPROPERTY(EditAnywhere)
	// float DiscountFactor = 0.95;
	//
	// UPROPERTY(EditAnywhere)
	// float ExplorationRate = 0.2;
	//
	// UPROPERTY(EditAnywhere)
	// int32 TotalEpisodes = 1000;
	//
	// int32 Episode = 0;
	//
	// // NumStates And NumActions Implementation in Begin
	// float Q[][];
	//
	// int ChooseAction(int State) const;
	// void UpdateQ(int State, int Action, float Reward, int NextState);
	// void QLearning();
};