// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonController.h"

#include "GameFramework/CharacterMovementComponent.h"

///
/// NOT USED IN THE PROJECT
///	THIS WAS CREATED FOR TESTING PURPOSES ONLY WHEN CREATING THE DUAL VIEW CHARACTER CONTROLLER
///

// Sets default values
AThirdPersonController::AThirdPersonController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boomArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom Arm"));
	boomArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	boomArm->TargetArmLength = 300.0f;
	boomArm->bUsePawnControlRotation = true;

	playerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	playerCamera->AttachToComponent(boomArm, FAttachmentTransformRules::KeepRelativeTransform);
	playerCamera->bUsePawnControlRotation = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 540, 0);

	turnRate = 45;
	lookRate = 45;

	sprintSpeed = 1000;
	walkSpeed = 400;
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

// Called when the game starts or when spawned
void AThirdPersonController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThirdPersonController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AThirdPersonController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Mouse Input
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AThirdPersonController::LookRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AThirdPersonController::LookUp);
	//WASD Input
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AThirdPersonController::WalkForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AThirdPersonController::WalkRight);

	//action mappings are either on or off and have a value of either 0 or 1
	//Jump
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AThirdPersonController::PlayerJump);
	//Crouch
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &AThirdPersonController::CrouchBegin);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &AThirdPersonController::CrouchEnd);
	//Sprint
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AThirdPersonController::SprintStart);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AThirdPersonController::SprintEnd);
}

void AThirdPersonController::LookUp(float axisValue)
{
	this->AddControllerPitchInput(axisValue * GetWorld()->GetDeltaSeconds() * turnRate);
}

void AThirdPersonController::LookRight(float axisValue)
{
	this->AddControllerYawInput(axisValue * GetWorld()->GetDeltaSeconds() * turnRate);
}

void AThirdPersonController::WalkForward(float axisValue)
{
	if(Controller != NULL && axisValue != 0)
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yaw(0, rotation.Yaw, 0);
		const FVector direction = FRotationMatrix(yaw).GetUnitAxis(EAxis::X);

		AddMovementInput(direction, axisValue);
	}

}

void AThirdPersonController::WalkRight(float axisValue)
{
	if (Controller != NULL && axisValue != 0)
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yaw(0, rotation.Yaw, 0);
		const FVector direction = FRotationMatrix(yaw).GetUnitAxis(EAxis::Y);

		AddMovementInput(direction, axisValue);
	}
}

void AThirdPersonController::PlayerJump()
{
	this->Jump();
}

void AThirdPersonController::CrouchBegin()
{
	this->Crouch();
}

void AThirdPersonController::CrouchEnd()
{
	this->UnCrouch();
}

void AThirdPersonController::SprintStart()
{
	GetCharacterMovement()->MaxWalkSpeed = sprintSpeed;
}

void AThirdPersonController::SprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}
