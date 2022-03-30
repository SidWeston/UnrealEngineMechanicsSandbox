// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractButton.h"

///
/// NOT USED IN THE PROJECT
///	THIS WAS CREATED FOR TESTING PURPOSES ONLY WHEN CREATING THE ACTUAL BUTTON COMPONENT
///	THIS IS AN ACTOR, NOT AN ACTOR COMPONENT SO CANNOT BE ADDED TO OTHER ACTORS AND THEREFORE DOESNT WORK
///

// Sets default values
AInteractButton::AInteractButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create mesh components
	//button background will likely be empty, is just used so the button itself isnt the root component
	buttonBackground = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button Back Plate"));
	buttonBackground->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	buttonObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button Object"));
	buttonObject->AttachToComponent(buttonBackground, FAttachmentTransformRules::KeepWorldTransform);

	//create interact component
	interactionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));
	//bind the interact function to button pressed
	interactionComponent->FInteract.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(AInteractButton, OnButtonPressed));

	buttonPressed = false; //button starts off not pressed

}

// Called when the game starts or when spawned
void AInteractButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractButton::OnButtonPressed()
{
	//swaps the boolean between on and off each time the button is pressed
	if(buttonPressed)
	{
		buttonPressed = false;
	}
	else if(!buttonPressed)
	{
		buttonPressed = true;
	}
}
