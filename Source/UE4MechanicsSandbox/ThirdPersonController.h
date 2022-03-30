// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ThirdPersonController.generated.h"

///
/// NOT USED IN THE PROJECT
///	THIS WAS CREATED FOR TESTING PURPOSES ONLY WHEN CREATING THE DUAL VIEW CHARACTER CONTROLLER
///

UCLASS()
class UE4MECHANICSSANDBOX_API AThirdPersonController : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AThirdPersonController();

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* boomArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* playerCamera;

	//input functions
	//Mouse/Camera Input
	void LookUp(float axisValue);
	void LookRight(float axisValue);
	//WASD/Movement Input
	void WalkForward(float axisValue);
	void WalkRight(float axisValue);
	//Action Inputs
	void PlayerJump();
	void CrouchBegin();
	void CrouchEnd();
	void SprintStart();
	void SprintEnd();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float sprintSpeed;
	float walkSpeed;

	float turnRate;
	float lookRate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
