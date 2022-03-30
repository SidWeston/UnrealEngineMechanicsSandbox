// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractButtonComp.h"

// Sets default values for this component's properties
UInteractButtonComp::UInteractButtonComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//create mesh components
	buttonObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button"));
	if(GetOwner()) //if component is added to an actor,
	{
		//will attempt to attach the button object to the root component of the owner actor
		buttonObject->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}

	//create interaction component
	interactionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	//bind interact function to button pressed
	interactionComponent->FInteract.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UInteractButtonComp, OnButtonPressed));

	//button starts off not pressed
	buttonPressed = false;

	// ...
}


// Called when the game starts
void UInteractButtonComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractButtonComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractButtonComp::OnButtonPressed()
{
	//swaps button pressed value between true and false when the button is pressed
	if(!buttonPressed)
	{
		buttonPressed = true;
	}
	else if(buttonPressed)
	{
		buttonPressed = false;
	}
}

