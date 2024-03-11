#include "DrivingRLTrainer.h"
#include "LearningAgentsHelpers.h"
#include "LearningAgentsRewards.h"
#include "LearningAgentsCompletions.h"
#include "Vehicle.h"

void UDrivingRLTrainer::BeginPlay()
{
	Super::BeginPlay();

	TrackSplineComp = Cast<USplineComponent>(TrackSpline);
}

void UDrivingRLTrainer::SetupRewards_Implementation()
{
	Super::SetupRewards_Implementation();

	OffTrackPenalty = OffTrackPenalty->AddPlanarPositionDifferencePenalty(this, FName("OffTrackPenalty"),
		1.0f, 100.0f, 800.0f);

	VelocityAlongTrackReward = VelocityAlongTrackReward->AddScalarVelocityReward(this, FName("SpeedReward"));

	SplineComponentRewardHelper = SplineComponentRewardHelper->AddSplineComponentHelper(this,
		FName("SplineComponentHelper"));
}

void UDrivingRLTrainer::SetRewards_Implementation(const TArray<int32>&AgentIds)
{
	Super::SetRewards_Implementation(AgentIds);

	for (const int32 AgentId : AgentIds)
	{
		const AActor* AgentActor = Cast<AActor>(GetAgent(AgentId));
		const FVector AgentLocation = AgentActor->GetActorLocation();

		const FVector NearestPositionOnSpline = SplineComponentRewardHelper->GetNearestPositionOnSpline
		(AgentId, TrackSplineComp, AgentLocation);

		const float VelocityAlongSpline = SplineComponentRewardHelper->GetVelocityAlongSpline
		(AgentId, TrackSplineComp, AgentLocation, AgentActor->GetVelocity());
		
		OffTrackPenalty->SetPlanarPositionDifferencePenalty(AgentId, NearestPositionOnSpline,
			AgentLocation);

		VelocityAlongTrackReward->SetScalarVelocityReward(AgentId, VelocityAlongSpline);
	}
}

void UDrivingRLTrainer::SetupCompletions_Implementation()
{
	Super::SetupCompletions_Implementation();

	OffTrackTermination = OffTrackTermination->AddPlanarPositionDifferenceCompletion(this,
		FName("OffTrackCompletion"), 1200.0f);
}

void UDrivingRLTrainer::SetCompletions_Implementation(const TArray<int32>&AgentIds)
{
	Super::SetCompletions_Implementation(AgentIds);

	for (const int32 AgentId : AgentIds)
	{
		const AActor* AgentActor = Cast<AActor>(GetAgent(AgentId));
		const FVector AgentLocation = AgentActor->GetActorLocation();

		const FVector NearestPositionOnSpline = SplineComponentRewardHelper->GetNearestPositionOnSpline
		(AgentId, TrackSplineComp, AgentLocation);

		OffTrackTermination->SetPlanarPositionDifferenceCompletion(AgentId, NearestPositionOnSpline,
			AgentLocation);
	}
}

void UDrivingRLTrainer::ResetEpisodes_Implementation(const TArray<int32>&AgentIds)
{
	Super::ResetEpisodes_Implementation(AgentIds);

	for (const int32 AgentId : AgentIds)
	{
		AVehicle* AgentVehicle = Cast<AVehicle>(GetAgent(AgentId));
		AgentVehicle->ResetToRandomPointOnSpline(TrackSplineComp);
	}
}