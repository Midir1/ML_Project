// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CubeMovement.generated.h"

UCLASS()
class ML_PROJECT_API ACubeMovement : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACubeMovement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
	
	float Horizontal = 0.0f;
	float Vertical = 0.0f;
	FVector Movement = FVector::Zero();
};