#include "Vehicle.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "LearningAgentsManager.h"
#include "Components/SplineComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void AVehicle::ResetToRandomPointOnSpline(const USplineComponent* SplineComp)
{
	const float RandomDistance = FMath::RandRange(0.0f, 1.0f) * SplineComp->GetSplineLength();

	const FVector LocationAtDistanceAlongSpline = SplineComp->GetLocationAtDistanceAlongSpline(RandomDistance,
		ESplineCoordinateSpace::World);
	
	const FRotator RotationAtDistanceAlongSpline = SplineComp->GetRotationAtDistanceAlongSpline(RandomDistance,
		ESplineCoordinateSpace::World);

	const FVector RandomLocation = FVector(FMath::RandRange(-0.5f, 0.5f) * 1200.0f,
		FMath::RandRange(-0.5f, 0.5f) * 1200.0f,50.0f) + LocationAtDistanceAlongSpline;

	const FRotator RandomRotator = FRotator(0.0f, 0.0f, FMath::RandRange(-0.5f, 0.5f)
		* 1200.0f + RotationAtDistanceAlongSpline.Yaw);

	const FTransform RandomTransform(RandomRotator, RandomLocation, FVector::One());
	SetActorTransform(RandomTransform);

	//Interrupt Angular & Linear Movement
	GetMesh()->SetPhysicsAngularVelocityInDegrees(FVector::Zero());
	GetMesh()->SetPhysicsLinearVelocity(FVector::Zero());
}

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