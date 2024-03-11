#pragma once

#include "CoreMinimal.h"
#include "LearningAgentsInteractor.h"
#include "DrivingInteractor.generated.h"

class USplineComponent;
class UFloatAction;
class USplineComponentHelper;
class UPlanarVelocityObservation;
class UAngleObservation;
class UPlanarDirectionObservation;
class UPlanarPositionObservation;

UCLASS()
class ML_PROJECT_API UDrivingInteractor : public ULearningAgentsInteractor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FVector GoalLocation;

	UPROPERTY(BlueprintReadWrite)
	AActor* TrackSpline;

protected:
	virtual void BeginPlay() override;
	virtual void SetupObservations_Implementation() override;
	virtual void SetObservations_Implementation(const TArray<int32>& AgentIds) override;
	virtual void SetupActions_Implementation() override;
	virtual void GetActions_Implementation(const TArray<int32>& AgentIds) override;

private:
	UPlanarPositionObservation* TrackPositionObservation = nullptr;
	UPlanarDirectionObservation* TrackDirectionObservation = nullptr;
	UAngleObservation* TrackGlobalPositionObservation = nullptr;
	UPlanarVelocityObservation* CarVelocityObservation = nullptr;
	USplineComponentHelper* SplineComponentHelper = nullptr;
	USplineComponent* TrackSplineComp = nullptr;

	UFloatAction* ThrottleBrakeAction = nullptr;
	UFloatAction* SteeringAction = nullptr;
};