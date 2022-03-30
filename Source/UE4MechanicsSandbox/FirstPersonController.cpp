// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonController.h"

#include "GameFramework/CharacterMovementComponent.h"

///
/// NOT USED IN THE PROJECT
///	THIS WAS CREATED FOR TESTING PURPOSES ONLY WHEN CREATING THE DUAL VIEW CHARACTER CONTROLLER
///


// Sets default values
AFirstPersonController::AFirstPersonController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	playerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	playerCamera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	this->GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	this->GetCharacterMovement()->MaxWalkSpeed = 400;
	playerCamera->bUsePawnControlRotation = true;
	//set default values for movement component when the first person controller is generated
	walkSpeed = 400;

	sprintSpeed = 1000;

}

// Called when the game starts or when spawned
void AFirstPersonController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFirstPersonController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFirstPersonController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind input events to call functions
	//Axis mappings are buttons which are held and can have a value between -1 and 1
	//Mouse Input
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AFirstPersonController::LookRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AFirstPersonController::LookUp);
	//WASD Input
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AFirstPersonController::WalkForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AFirstPersonController::WalkRight);

	//action mappings are either on or off and have a value of either 0 or 1
	//Jump
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AFirstPersonController::PlayerJump);
	//Crouch
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &AFirstPersonController::CrouchBegin);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &AFirstPersonController::CrouchEnd);
	//Sprint
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AFirstPersonController::SprintStart);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AFirstPersonController::SprintEnd);
	//Player Abilities
	PlayerInputComponent->BindAction(TEXT("AbilityOne"), IE_Pressed, this, &AFirstPersonController::UseAbilityOne);
	PlayerInputComponent->BindAction(TEXT("AbilityTwo"), IE_Pressed, this, &AFirstPersonController::UseAbilityTwo);

}

//uses mouseY 
void AFirstPersonController::LookUp(float axisValue)
{
	this->AddControllerPitchInput(axisValue);
}

//uses mouseX
void AFirstPersonController::LookRight(float axisValue)
{
	this->AddControllerYawInput(axisValue);
}

//W + S
void AFirstPersonController::WalkForward(float axisValue)
{
	this->AddMovementInput(this->GetActorForwardVector() * axisValue);
}

//A + D
void AFirstPersonController::WalkRight(float axisValue)
{
	this->AddMovementInput(this->GetActorRightVector() * axisValue);
}

//Spacebar
void AFirstPersonController::PlayerJump()
{
	this->Jump();
}

//Left Control Pressed
void AFirstPersonController::CrouchBegin()
{
	this->Crouch();
}

//Left Control Released
void AFirstPersonController::CrouchEnd()
{
	this->UnCrouch();
}

//left Shift Pressed
void AFirstPersonController::SprintStart()
{
	GetCharacterMovement()->MaxWalkSpeed = sprintSpeed;
}

//Left Shift Released
void AFirstPersonController::SprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void AFirstPersonController::UseAbilityOne()
{
	//currently toggles between allowing double jumps and not allowing it
	if (JumpMaxCount != 2)
	{
		JumpMaxCount = 2;
	}
	else
	{
		JumpMaxCount = 1;
	}
}

void AFirstPersonController::UseAbilityTwo()
{
	//TODO
}

