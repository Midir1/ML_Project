#pragma once

#include "CoreMinimal.h"
#include "LearningAgentsTrainer.h"
#include "DrivingRLTrainer.generated.h"

UCLASS()
class ML_PROJECT_API UDrivingRLTrainer : public ULearningAgentsTrainer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	AActor* TrackSpline;

protected:
	virtual void SetupRewards_Implementation() override;
	virtual void SetRewards_Implementation(const TArray<int32>& AgentIds) override;
	virtual void SetupCompletions_Implementation() override;
	virtual void SetCompletions_Implementation(const TArray<int32>& AgentIds) override;

private:
	// TODO : Find include Add Var called AddPlanar  
};