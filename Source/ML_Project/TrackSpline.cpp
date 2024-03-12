#include "TrackSpline.h"
#include "Components/SplineComponent.h"

ATrackSpline::ATrackSpline()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
}

USplineComponent* ATrackSpline::GetSplineComponent() const
{
	return SplineComp;
}
