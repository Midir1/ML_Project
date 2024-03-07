#include "DrivingInteractor.h"
#include "LearningAgentsObservations.h"
#include "LearningAgentsHelpers.h"
#include "LearningAgentsActions.h"
#include "ChaosWheeledVehicleMovementComponent.h"

void UDrivingInteractor::SetupObservations_Implementation()
{
	Super::SetupObservations_Implementation();

	TrackPositionObservation = TrackPositionObservation->AddPlanarPositionObservation(this,
		FName("TrackPositionObservation"));

	TrackDirectionObservation = TrackDirectionObservation->AddPlanarDirectionObservation(this,
		FName("TrackDirectionObservation"));

	TrackGlobalPositionObservation = TrackGlobalPositionObservation->AddAngleObservation(this,
		FName("TrackGlobalPositionObservation"));

	CarVelocityObservation = CarVelocityObservation->AddPlanarVelocityObservation(this,
		FName("CarVelocityObservation"));

	SplineComponentHelper = SplineComponentHelper->AddSplineComponentHelper(this,
		FName("SplineComponentHelper"));
}

void UDrivingInteractor::SetObservations_Implementation(const TArray<int32>& AgentIds)
{
	Super::SetObservations_Implementation(AgentIds);

	for (const int32 AgentId : AgentIds)
	{
		const AActor* AgentActor = Cast<AActor>(GetAgent(AgentId));

		const FVector AgentLocation = AgentActor->GetActorLocation();
		const FRotator AgentRotation = AgentActor->GetActorRotation();

		const USplineComponent* TrackSplineComp = Cast<USplineComponent>(TrackSpline);

		const float DistanceAlongSpline = SplineComponentHelper->GetDistanceAlongSplineAtPosition(AgentId, TrackSplineComp, AgentLocation);

		const FVector PositionAlongSpline = SplineComponentHelper->GetPositionAtDistanceAlongSpline(AgentId, TrackSplineComp, DistanceAlongSpline);
		const FVector DirectionAlongSpline = SplineComponentHelper->GetDirectionAtDistanceAlongSpline(AgentId, TrackSplineComp, DistanceAlongSpline);
		const float AngleAlongSpline = SplineComponentHelper->GetProportionAlongSplineAsAngle(AgentId, TrackSplineComp, DistanceAlongSpline);

		TrackPositionObservation->SetPlanarPositionObservation(AgentId, PositionAlongSpline, AgentLocation, AgentRotation);
		TrackDirectionObservation->SetPlanarDirectionObservation(AgentId, DirectionAlongSpline, AgentRotation);
		TrackGlobalPositionObservation->SetAngleObservation(AgentId, AngleAlongSpline);
		CarVelocityObservation->SetPlanarVelocityObservation(AgentId,  AgentActor->GetVelocity(), AgentRotation);
	}
}

void UDrivingInteractor::SetupActions_Implementation()
{
	Super::SetupActions_Implementation();

	ThrottleBrakeAction = ThrottleBrakeAction->AddFloatAction(this, FName("ThrottleBrakeAction"));
	SteeringAction = SteeringAction->AddFloatAction(this, FName("SteeringAction"));
}

void UDrivingInteractor::GetActions_Implementation(const TArray<int32>& AgentIds)
{
	Super::GetActions_Implementation(AgentIds);

	for (const int32 AgentId : AgentIds)
	{
		UChaosVehicleMovementComponent* VehicleMovementComponent = Cast<UChaosVehicleMovementComponent>(GetAgent(AgentId));;

		const float ThrottleValue = ThrottleBrakeAction->GetFloatAction(AgentId);
		
		if(ThrottleValue > 0.0f)
		{
			VehicleMovementComponent->SetThrottleInput(ThrottleValue);
			VehicleMovementComponent->SetBrakeInput(0.0f);
		}
		else
		{
			VehicleMovementComponent->SetThrottleInput(0.0f);
			VehicleMovementComponent->SetBrakeInput(-ThrottleValue);
		}
		
		VehicleMovementComponent->SetSteeringInput(SteeringAction->GetFloatAction(AgentId));
	}
}