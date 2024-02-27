#include "DrivingInteractor.h"
#include "LearningAgentsObservations.h"

void UDrivingInteractor::SetupObservations_Implementation()
{
	Super::SetupObservations_Implementation();

	GoalLocationObservation =
		GoalLocationObservation->AddPositionObservation(this, FName("GoalLocation"), 10000.0f);
	
}

void UDrivingInteractor::SetObservations_Implementation(const TArray<int32>& AgentIds)
{
	Super::SetObservations_Implementation(AgentIds);

	for (const int32 AgentId : AgentIds)
	{
		FTransform AgentTransform = Cast<AActor>(GetAgent(AgentId))->GetActorTransform();

		GoalLocationObservation->SetPositionObservation(AgentId, GoalLocation,
			AgentTransform.GetLocation(), AgentTransform.GetRotation().Rotator());
	}
}
