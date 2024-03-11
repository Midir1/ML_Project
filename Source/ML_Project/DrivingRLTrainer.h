#pragma once

#include "CoreMinimal.h"
#include "LearningAgentsTrainer.h"
#include "DrivingRLTrainer.generated.h"

class USplineComponent;
class UScalarVelocityReward;
class USplineComponentHelper;
class UPlanarPositionDifferencePenalty;
class UPlanarPositionDifferenceCompletion;

UCLASS()
class ML_PROJECT_API UDrivingRLTrainer : public ULearningAgentsTrainer
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	USplineComponent* TrackSpline;

protected:
	virtual void SetupRewards_Implementation() override;
	virtual void SetRewards_Implementation(const TArray<int32>& AgentIds) override;
	virtual void SetupCompletions_Implementation() override;
	virtual void SetCompletions_Implementation(const TArray<int32>& AgentIds) override;
	virtual void ResetEpisodes_Implementation(const TArray<int32>&AgentIds) override;

private:
	UPlanarPositionDifferencePenalty* OffTrackPenalty = nullptr;
	UScalarVelocityReward* VelocityAlongTrackReward = nullptr;
	USplineComponentHelper* SplineComponentRewardHelper = nullptr;
	UPlanarPositionDifferenceCompletion* OffTrackTermination = nullptr;
};