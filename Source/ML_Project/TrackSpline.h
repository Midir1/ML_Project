#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrackSpline.generated.h"

class USplineComponent;

UCLASS()
class ML_PROJECT_API ATrackSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	ATrackSpline();
	
	USplineComponent* GetSplineComponent() const;

private:
	USplineComponent* SplineComp = nullptr;
};