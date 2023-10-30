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
	
	Vertical = GetInputAxisValue(FName("MoveForward"));
	Horizontal = GetInputAxisValue(FName("MoveRight"));

	Movement = (GetActorLocation() + Horizontal + Vertical) * DeltaTime;

	GEngine->AddOnScreenDebugMessage(5,-1,FColor::Cyan,
		FString::Printf(TEXT("Movement : %f, %f, %f"), Movement.X, Movement.Y, Movement.Z));
	
	SetActorLocation(Movement);
}

void ACubeMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}