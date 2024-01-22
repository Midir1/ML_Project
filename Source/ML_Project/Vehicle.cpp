#include "Vehicle.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"

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

	// Q[NumStates][NumActions] = {0};
	// QLearning();
}

void AVehicle::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	// QLearning();
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

// int AVehicle::ChooseAction(const int State) const
// {
// 	const int32 Rand = FMath::RandRange(0, 2147483647);
// 	
// 	if (Rand / 2147483647 < ExplorationRate)
// 	{
// 		return Rand % NumActions;
// 	}
// 	
// 	int BestAction = 0;
// 	
// 	for (int i = 1; i < NumActions; ++i)
// 	{
// 		if (Q[State][i] > Q[State][BestAction])
// 		{
// 			BestAction = i;
// 		}
// 	}
// 	return BestAction;
// }
//
// void AVehicle::UpdateQ(const int State, const int Action, const float Reward, const int NextState)
// {
// 	Q[State][Action] = Q[State][Action] + LearningRate * (Reward + DiscountFactor
// 		* Q[NextState][ChooseAction(NextState)] - Q[State][Action]);
// }
//
// void AVehicle::QLearning()
// {
// 	if (Episode < TotalEpisodes)
// 	{
// 		int32 CurrentState =  FMath::RandRange(0, 2147483647) % NumStates;
// 		
// 		while (CurrentState != NumStates - 1)
// 		{  
// 			const int Action = ChooseAction(CurrentState);
// 			
// 			const int NextState = (Action == 0) ? CurrentState + 1 : CurrentState + 2;
// 			const float Reward = (NextState == NumStates - 1) ? 100.0 : 0.0;
// 			
// 			UpdateQ(CurrentState, Action, Reward, NextState);
// 			CurrentState = NextState;
// 		}
//
// 		++Episode;
// 	}
// }