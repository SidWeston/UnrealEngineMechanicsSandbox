// Fill out your copyright notice in the Description page of Project Settings.


#include "DualViewCharacterController.h"
#include "InteractionComponent.h"
#include "InspectableItem.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ADualViewCharacterController::ADualViewCharacterController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//set initial camera mode
	currentCameraMode = FirstPerson;

	//setup first person camera
	firstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	firstPersonCamera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	firstPersonCamera->bUsePawnControlRotation = true;

	//setup third person camera
	//third person camera rotates around a spring arm component
	boomArmTP = CreateDefaultSubobject <USpringArmComponent>(TEXT("Boom Arm"));
	boomArmTP->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	boomArmTP->TargetArmLength = 300.0f;
	boomArmTP->bUsePawnControlRotation = true;
	thirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Third Person Camera"));
	thirdPersonCamera->AttachToComponent(boomArmTP, FAttachmentTransformRules::KeepRelativeTransform);
	thirdPersonCamera->bUsePawnControlRotation = true;
	thirdPersonCamera->SetActive(false);

	//set the rotation rate for the character controller
	GetCharacterMovement()->RotationRate = FRotator(0, 540, 0);
	turnRate = 45;

	//set default values for walk and sprint speed, can be changed in the editor if needed
	walkSpeed = 400;
	sprintSpeed = 1000;
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed; //set the player move speed to be the walk speed, assumes the player starts off walking not sprinting

	//create the widget interaction component
	widgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Widget Interaction"));
	widgetInteraction->AttachToComponent(firstPersonCamera, FAttachmentTransformRules::KeepRelativeTransform);

	//create grapple hook component
	grappleHook = CreateDefaultSubobject<UCableComponent>(TEXT("Grapple Hook"));
	grappleHook->AttachToComponent(firstPersonCamera, FAttachmentTransformRules::KeepWorldTransform);
	grappleHook->SetHiddenInGame(true); //starts off hidden so the cable isnt dangling around whilst not being used

	//create stamina values
	maxStamina = 100;
	staminaDrainRate = 10; //roughly 10 seconds of sprinting before stamina runs out

	//create the sphere component to use as the item inspection location
	itemInspectLocation = CreateDefaultSubobject<USphereComponent>(TEXT("Item Inspect Location"));
	itemInspectLocation->AttachToComponent(firstPersonCamera, FAttachmentTransformRules::KeepWorldTransform);
	//create physics handle
	physicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Handle"));
}

// Called when the game starts or when spawned
void ADualViewCharacterController::BeginPlay()
{
	Super::BeginPlay();

	//set the player stamina to be the max stamina initially
	stamina = maxStamina;

}

// Called every frame
void ADualViewCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if the grapple hook is attached to an object and the player is in the air
	if(hookAttached && !this->GetCharacterMovement()->IsMovingOnGround())
	{
		FVector grappleHookStart = GetActorLocation() -= swingPoint; //get the start location of the grapple hook
		const float playerVelocity = FVector::DotProduct(this->GetVelocity(), grappleHookStart); //calculate the player velocity 
		grappleHookStart.Normalize(); //normalize the grapple hook start vector
		grappleHookStart *= playerVelocity;
		FVector forceToAdd = grappleHookStart *= -10.0f;
		if(playerVelocity < maxSwingSpeed) //if the player velocity is higher than the maximum allowed swing speed,
		{
			forceToAdd /= 1.5f; //lower the force added to the player
		}
		this->GetCharacterMovement()->AddForce(forceToAdd); //add force to the player to make them swing
	}

	//when the player is on the floor, reset the jumps and dashes
	if(this->GetCharacterMovement()->IsMovingOnGround())
	{
		hasJumped = false;
		hasDashed = false;
	}

	if(isSprinting) //when the player is sprinting (holding left shift)
	{
		stamina -= staminaDrainRate * DeltaTime; //drains the players stamina, multiplied by delta time so it drains at the same rate regardless of frame rate
		if(stamina <= 0)
		{
			this->GetCharacterMovement()->MaxWalkSpeed = walkSpeed; //if the player's stamina runs out, will set their move speed to walking speed
			isSprinting = false; //no longer sprinting so stamina regeneration can begin
		}
	}
	else if(!isSprinting) //if they are no longer sprinting
	{
		if(stamina < maxStamina) //if stamina is less than max
		{
			stamina += staminaDrainRate * DeltaTime; //regenerates the players stamina at the same rate as it was drained	
		}
		else if(stamina > maxStamina) //if it reaches more than max
		{
			stamina = maxStamina; //if the player's stamina becomes greater than what it should be, it will be set to the value of max stamina
		}
		
	}

}

// Called to bind functionality to input
void ADualViewCharacterController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind input events to call functions
	//Axis mappings are buttons which are held and can have a value between -1 and 1
	//Mouse Input
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &ADualViewCharacterController::LookRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ADualViewCharacterController::LookUp);
	//WASD Input
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ADualViewCharacterController::WalkForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ADualViewCharacterController::WalkRight);

	//action mappings are either on or off and have a value of either 0 or 1
	//Jump
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ADualViewCharacterController::PlayerJump);
	//Sprint
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ADualViewCharacterController::SprintStart);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ADualViewCharacterController::SprintEnd);
	//Player Abilities
	PlayerInputComponent->BindAction(TEXT("AbilityOne"), IE_Pressed, this, &ADualViewCharacterController::UseAbilityOne);
	//Change the player camera
	PlayerInputComponent->BindAction(TEXT("CameraChange"), IE_Pressed, this, &ADualViewCharacterController::ChangeCamera);
	//interact function fires raycast
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ADualViewCharacterController::DoLineTrace);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Released, this, &ADualViewCharacterController::ReleaseWidgetInteraction);
	//fire function fires longer raycast for use with grapple hook
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ADualViewCharacterController::FireGrappleHook);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ADualViewCharacterController::ReleaseGrappleHook);
}

void ADualViewCharacterController::LookUp(float axisValue)
{
	if(!isInspecting) //if the player isnt inspecting the camera should work normally
	{
		switch (currentCameraMode)//change which camera is affected by mouse movement 
		{
		case FirstPerson:
		{
			this->AddControllerPitchInput(axisValue);
			physicsHandle->SetTargetLocation(firstPersonCamera->GetComponentLocation() + (firstPersonCamera->GetForwardVector() * 200)); //set the target location that the physics handle will set the objects to when they are picked up
			break;
		}
		case ThirdPerson:
		{
			this->AddControllerPitchInput(axisValue * GetWorld()->GetDeltaSeconds() * turnRate);
			physicsHandle->SetTargetLocation(thirdPersonCamera->GetComponentLocation() + (thirdPersonCamera->GetForwardVector() * 540)); //set the target location that the physics handle will set the objects to when they are picked up
			break;
		}
		default:
		{
			break;
		}
		}
	}
	else if (isInspecting) //if the player is inspecting an object the mouse should rotate the object, not the camera
	{
		FRotator newItemRotationY(0, 0, axisValue);
		newItemRotationY += itemToInspect->GetActorRotation();
		itemToInspect->SetActorRotation(newItemRotationY);
	}
}

void ADualViewCharacterController::LookRight(float axisValue)
{
	if(!isInspecting) //if an object is not being inspected the camera should work like normal
	{
		switch (currentCameraMode) //change which camera is affected by mouse movement 
		{
		case FirstPerson:
		{
			this->AddControllerYawInput(axisValue);
			physicsHandle->SetTargetLocation(firstPersonCamera->GetComponentLocation() + (firstPersonCamera->GetForwardVector() * 200)); //set the target location that the physics handle will set the objects to when they are picked up
			break;
		}
		case ThirdPerson:
		{
			this->AddControllerYawInput(axisValue * GetWorld()->GetDeltaSeconds() * turnRate);
			physicsHandle->SetTargetLocation(thirdPersonCamera->GetComponentLocation() + (thirdPersonCamera->GetForwardVector() * 540)); //set the target location that the physics handle will set the objects to when they are picked up
			break;
		}
		default:
		{
			break;
		}
		}
	}
	else if(isInspecting) //if there is an object being inspected the mouse should rotate the object, not the camera
	{
		FRotator newItemRotationY(0, axisValue *= -1, 0);
		newItemRotationY += itemToInspect->GetActorRotation();
		itemToInspect->SetActorRotation(newItemRotationY);
	}


}

void ADualViewCharacterController::WalkForward(float axisValue)
{
	if(!isInspecting) //player cant move if they are inspecting an object
	{
		switch (currentCameraMode) //change how the player moves based on the current active camera
		{
		case FirstPerson:
		{
			this->AddMovementInput(this->GetActorForwardVector() * axisValue);
			break;
		}
		case ThirdPerson:
		{
			if (Controller != NULL && axisValue != 0)
			{

				const FRotator rotation = Controller->GetControlRotation(); //get the character's rotation
				const FRotator yaw(0, rotation.Yaw, 0); //create a FRotator that only has the yaw value of the controller
				const FVector direction = FRotationMatrix(yaw).GetUnitAxis(EAxis::X); //gets the direction on the x axis(forward)

				AddMovementInput(direction, axisValue); //adds movement in the forward direction
			}

			break;
		}
		default:
		{
			break;
		}
		}
	}
}
void ADualViewCharacterController::WalkRight(float axisValue)
{
	if(!isInspecting) //player cant move if they are inspecting an object
	{
		switch (currentCameraMode)//change how the player moves based on the current active camera
		{
		case FirstPerson:
		{
			this->AddMovementInput(this->GetActorRightVector() * axisValue);
			break;
		}
		case ThirdPerson:
		{
			if (Controller != NULL && axisValue != 0)
			{
				const FRotator rotation = Controller->GetControlRotation();
				const FRotator yaw(0, rotation.Yaw, 0);
				const FVector direction = FRotationMatrix(yaw).GetUnitAxis(EAxis::Y);

				AddMovementInput(direction, axisValue);
			}
			break;
		}
		default:
		{
			break;
		}
		}
	}
}
void ADualViewCharacterController::PlayerJump()
{
	//if the player hasnt already jumped
	if(!hasJumped)
	{
		this->LaunchCharacter(this->GetActorUpVector() * jumpHeight, false, true); //launch the player up, overriding any momentum on the up axis
		hasJumped = true; //stop the player from jumping twice to prevent infinite jumping, will be reset when the player touches the floor
	}
}

void ADualViewCharacterController::SprintStart()
{
	if(stamina > 0) //if the player has stamina left they can start sprinting
	{
		GetCharacterMovement()->MaxWalkSpeed = sprintSpeed; //set the movement speed to the sprinting speed
		isSprinting = true; 
	}
}
void ADualViewCharacterController::SprintEnd()
{
	//when sprinting stops reset movement speed to walk speed
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	isSprinting = false;
}
void ADualViewCharacterController::UseAbilityOne() //dash forward
{
	if(!hasDashed) //if the player hasnt already dashed
	{
		//create a vector to launch the character with
		FVector dashVector;
		if(this->GetCharacterMovement()->IsMovingOnGround())
		{
			//if the player is on the ground add a slight upwards direction to the vector to stop the player dragging on the ground
			dashVector = this->GetActorForwardVector() * dashLength + this->GetActorUpVector() * 200;
		}
		else
		{
			dashVector = this->GetActorForwardVector() * dashLength; //if the player is already in the air no need to boost them up slightly
		}
		this->LaunchCharacter(dashVector, false, false); //launch the character forward using the vector
		hasDashed = true; //the player cannot dash again until this is reset, reset when the player touches the ground
	}
}

void ADualViewCharacterController::ChangeCamera()
{
	if(currentCameraMode == FirstPerson)//if in first person mode, change to third person
	{
		//sets the current camera to be inactive and changes to the other camera
		firstPersonCamera->SetActive(false);
		thirdPersonCamera->SetActive(true);
		//doesnt let the player mesh rotate when the camera is moved
		bUseControllerRotationYaw = false;
		//when the character moves, the mesh will rotate in the direction the player is moving
		GetCharacterMovement()->bOrientRotationToMovement = true;
		//sets the camera mode to be third person
		currentCameraMode = ThirdPerson;
	}
	else if(currentCameraMode == ThirdPerson) //if in third person mode, change to first person
	{
		//sets the current camera to be inactive and changes to the other camera
		thirdPersonCamera->SetActive(false);
		firstPersonCamera->SetActive(true);
		//lets the player mesh be rotated when the camera is moved
		bUseControllerRotationYaw = true;
		//since the mesh is rotated when the camera is moved, doesnt need to be rotated when the character itself is moved
		GetCharacterMovement()->bOrientRotationToMovement = false;
		//sets the camera mode to be first person
		currentCameraMode = FirstPerson;
	}
}

void ADualViewCharacterController::DoLineTrace()
{
	widgetInteraction->PressPointerKey(EKeys::LeftMouseButton); //when the interact key is pressed, it acts as if the left mouse button is being pressed to enable interaction with ui elements

	if(!isInspecting) //if the player isnt inspecting anything do a linetrace/raycast like normal
	{
		//create a hitresult to output to
		FHitResult outHit;
		//Create start location for the raycast
		FVector startLocation;
		FVector endLocation; 
		switch(currentCameraMode) //change the origin of the raycast so it works in both first and third person mode
		{
		case FirstPerson:
			{
				startLocation = firstPersonCamera->GetComponentLocation();
				endLocation = ((firstPersonCamera->GetForwardVector() * 200.0f) + startLocation);
				break;
			}
		case ThirdPerson:
			{
				startLocation = thirdPersonCamera->GetComponentLocation();
				endLocation = ((thirdPersonCamera->GetForwardVector() * 540.0f) + startLocation); //third person camera needs extra linetrace distance as it is located further back on the player prefab
				break;
			}
		}
		//Create end location
		
		//Create collision parameters
		const FCollisionQueryParams collisionParams;

		//draw the line in the game for debug purposes
		DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, 1, 0, 1);

		//Start the line trace
		if (GetWorld()->LineTraceSingleByChannel(outHit, startLocation, endLocation, ECC_Visibility, collisionParams))
		{
			if (outHit.GetActor()->ActorHasTag("Interactable") || outHit.GetComponent()->ComponentHasTag("Interactable")) //checks if the actor/component that is hit is tagged as interactable
			{
				if (outHit.GetActor()->FindComponentByClass<UInteractionComponent>()) //checks if the object has the interaction component to avoid crashes
				{
					//need to determine whether it was an entire actor that was interactable or just a specific component
					if (outHit.GetActor()->ActorHasTag("Interactable")) //if the raycast hit a whole actor
					{
						outHit.GetActor()->FindComponentByClass<UInteractionComponent>()->ActivationFunction(outHit.GetActor()); //calls the activation function on the object which uses delegates	
					}
					else if (outHit.GetComponent()->ComponentHasTag("Interactable")) //if the raycast hit a specific component
					{
						outHit.GetActor()->FindComponentByClass<UInteractionComponent>()->ActivationFunction(outHit.GetComponent()); //calls the activation function on the object which uses delegates
					}

				}
			}
			else if (outHit.GetActor()->ActorHasTag("Item") || outHit.GetComponent()->ComponentHasTag("Item")) //if the line trace hit an item that the player can inspect
			{
				itemToInspect = Cast<AInspectableItem>(outHit.GetActor()); //attempts to cast to an inspectable item class using the object hit by the raycast
				if (itemToInspect != nullptr) //if the cast was successful itemToInspect wont be nullptr
				{
					itemToInspect->SetItemTransform(itemInspectLocation->GetComponentLocation(), itemToInspect->GetActorRotation());
					isInspecting = true;
				}
			}
			else if(outHit.GetActor()->ActorHasTag("Pickup") || outHit.GetComponent()->ComponentHasTag("Pickup")) //if the line trace hit an item which the player can pick up
			{
				physicsHandle->GrabComponentAtLocation(outHit.GetComponent(), "None", outHit.GetComponent()->GetComponentLocation()); //grab the item 
			}
		}
	}
	else if(isInspecting) //will only be true if item to inspect is a valid object so no need to check against nullptr
	{
		itemToInspect->itemTimeline->Reverse(); //place the item back by reversing timeline
		isInspecting = false; //no longer inspecting an item 
	}
}

void ADualViewCharacterController::ReleaseWidgetInteraction()
{
	//when the interact key is released
	widgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton); //release the left mouse button to stop interacting with any potential widgets 
	physicsHandle->ReleaseComponent(); //put down any objects that have been picked up
}

//this function fires a linetrace, similar to the interact function but this has different logic when it hits the relevant item,
//and the linetrace itself if longer, so a different function makes it easier
void ADualViewCharacterController::FireGrappleHook()
{
	//fire the grapple hook using a ray cast
	//create a hit result to output data to
	FHitResult outHit;
	//create start and end locations for the linetrace/raycast
	FVector startLocation;
	FVector endLocation;
	switch (currentCameraMode)//change the origin of the raycast depending on the current camera mode
	{
	case FirstPerson:
	{
		startLocation = firstPersonCamera->GetComponentLocation();
		endLocation = ((firstPersonCamera->GetForwardVector() * 1000.0f) + startLocation);
		break;
	}
	case ThirdPerson:
	{
		startLocation = thirdPersonCamera->GetComponentLocation();
		endLocation = ((thirdPersonCamera->GetForwardVector() * 1340.0f) + startLocation); //third person camera needs extra linetrace distance as it is located further back on the player prefab
		break;
	}
	}
	//Create collision parameters
	FCollisionQueryParams collisionParams;

	//draw the line in the game for debug purposes
	DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, 1, 0, 1);

	if (GetWorld()->LineTraceSingleByChannel(outHit, startLocation, endLocation, ECC_Visibility, collisionParams))
	{
		if (outHit.GetActor()->ActorHasTag("Swingable") || outHit.GetComponent()->ComponentHasTag("Swingable")) //checks if the actor/component that is hit is tagged as swingable
		{
			swingPoint = outHit.ImpactPoint; //sets swing point to the world location of where the linetrace hit
			grappleHook->SetHiddenInGame(false); //unhides the grapple hook
			hookAttached = true; //tells the tick function that the hook is attached
			//Spawn actor for the grapple hook location
			const FVector location = outHit.ImpactPoint; 
			const FRotator rotation(0, 0, 0); //0 frotator as it doesnt need a rotation 
			const FActorSpawnParameters spawnParam;
			ropeAttachPoint = GetWorld()->SpawnActor<ARopeAttachPoint>(location, rotation, spawnParam); //spawns an actor, which the grapple hook cable component will be attached to
			grappleHook->SetAttachEndTo(ropeAttachPoint, "None", "None"); //attach the end of the grapple hook
		}
	}
}

void ADualViewCharacterController::ReleaseGrappleHook()
{
	if(hookAttached == true) //will only proceed if the hook was actually attached to something previously
	{
		hookAttached = false; //hook is no longer attached
		grappleHook->SetHiddenInGame(true); //hide the hook
		ropeAttachPoint = nullptr; //remove the rope attach point
		if(!this->GetCharacterMovement()->IsMovingOnGround()) //if the character is not on the ground
		{
			this->Jump(); //gives the player a small boost when they release from the grapple hook
		}

	}
}



