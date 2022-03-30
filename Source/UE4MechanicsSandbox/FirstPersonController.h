// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "FirstPersonController.generated.h"

///
/// NOT USED IN THE PROJECT
///	THIS WAS CREATED FOR TESTING PURPOSES ONLY WHEN CREATING THE DUAL VIEW CHARACTER CONTROLLER
///

UCLASS()
class UE4MECHANICSSANDBOX_API AFirstPersonController : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFirstPersonController();

	//Input functions
	//Mouse/Camera input
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
	void UseAbilityOne();
	void UseAbilityTwo();

	//create a camera 
	UPROPERTY(EditAnywhere);
	UCameraComponent* playerCamera;

	UPROPERTY(EditAnywhere);
	float sprintSpeed;

	UPROPERTY(EditAnywhere);
	float walkSpeed;

	UPROPERTY(EditAnywhere);
	bool canDoubleJump;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
