// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractTestObject.h"

///
/// NOT USED IN THE PROJECT
///	THIS WAS CREATED FOR TESTING PURPOSES ONLY WHEN FIRST CREATING THE INTERACTION COMPONENT
///

// Sets default values
AInteractTestObject::AInteractTestObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	objMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object Mesh"));
	objMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Comp"));
	InteractionComponent->FInteract.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(AInteractTestObject, InteractWithObject));
	
}

// Called when the game starts or when spawned
void AInteractTestObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractTestObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractTestObject::InteractWithObject()
{
	UE_LOG(LogTemp, Warning, TEXT("Interacted with cube"));
}

