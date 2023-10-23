#include "CubeMovement.h"

ACubeMovement::ACubeMovement()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACubeMovement::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACubeMovement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float Vertical = GetInputAxisValue(FName("MoveForward"));
	float horizontal = GetInputAxisValue(FName("MoveRight"));

	const FVector Movement = GetActorLocation() + horizontal * DeltaTime + Vertical * DeltaTime;
	SetActorLocation(Movement);
	

}

void ACubeMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}