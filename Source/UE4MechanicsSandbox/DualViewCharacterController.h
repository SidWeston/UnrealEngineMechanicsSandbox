// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetInteractionComponent.h"
#include "CableComponent.h"
#include "RopeAttachPoint.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "DualViewCharacterController.generated.h"

//forward declare
class AInspectableItem;

UCLASS()
class UE4MECHANICSSANDBOX_API ADualViewCharacterController : public ACharacter
{
	GENERATED_BODY()

public:
	enum CameraMode //enum adds the possibility for more camera/movement modes to be added
	{
		FirstPerson,
		ThirdPerson,
	};

	// Sets default values for this character's properties
	ADualViewCharacterController();

	//prevents the player from double jumping or dashing - will be reset when the player hits the floor
	bool hasJumped, hasDashed;

	//the current camera mode of the player - third or first person
	CameraMode currentCameraMode;
	//create camera components
	UPROPERTY(VisibleAnywhere);
	UCameraComponent* firstPersonCamera;
	UPROPERTY(VisibleAnywhere);
	UCameraComponent* thirdPersonCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Input functions
	//Mouse/Camera input
	void LookUp(float axisValue);
	void LookRight(float axisValue);
	//WASD/Movement Input
	void WalkForward(float axisValue);
	void WalkRight(float axisValue);
	//Action Inputs
	//Jump
	void PlayerJump();
	//Sprint
	void SprintStart();
	void SprintEnd();
	//Player Abilities
	void UseAbilityOne();
	//Change camera mode between first and third person
	void ChangeCamera();

	//create a boom arm for the third person perspective
	UPROPERTY(VisibleAnywhere);
	USpringArmComponent* boomArmTP;
	float turnRate; //how fast the camera rotates around the player in third person

	UPROPERTY(EditAnywhere)
	float jumpHeight; //how high the player can jump
	UPROPERTY(EditAnywhere)
	float dashLength; //how far the player can dash forward

	//walking and sprinting speeds
	float walkSpeed, sprintSpeed;

	//widget interaction component will allow the player to interact with ui elements easier, such as the keypad door
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetInteractionComponent* widgetInteraction;

	//grapple hook
	UPROPERTY(VisibleAnywhere)
	UCableComponent* grappleHook; //the cable component to make it look like the player is attached to the object they will swing from
	bool hookAttached; //if the hook is currently attached to an object
	FVector swingPoint; //the location that the player will swing from
	UPROPERTY(EditAnywhere)
	float maxSwingSpeed; //the maximum swing speed of the player
	UPROPERTY(VisibleAnywhere)
	ARopeAttachPoint* ropeAttachPoint; //empty actor used to attach the end of the grapple hook to where the raycast lands

	bool isSprinting; //true if the player is currently sprinting
	//values for stamina, max stamina and how fast it drains
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float stamina; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float maxStamina;
	UPROPERTY(EditAnywhere)
	float staminaDrainRate; 

	//item inspection
	UPROPERTY(EditAnywhere)
	USphereComponent* itemInspectLocation; //sphere component used to determine where an inspectable item will travel to when hit by a raycast
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isInspecting; //if the player is currently inspecting an object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AInspectableItem* itemToInspect; //what item the player is currently inspecting

	//pick up/carry objects
	UPhysicsHandleComponent* physicsHandle; //physics handle to handle the physics of picking up objects
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//public functions
	void DoLineTrace(); //used for interaction (f/right click)
	void ReleaseWidgetInteraction(); //used to release the interaction with widgets when the interact key is released
	void FireGrappleHook(); //fires out a longer raycast, uses left click instead of the interact keys
	void ReleaseGrappleHook(); //releases the grappling hook when left click is released

};
