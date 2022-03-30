// Fill out your copyright notice in the Description page of Project Settings.


#include "InspectableItem.h"

// Sets default values
AInspectableItem::AInspectableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create mesh component
	itemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	itemMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	//create timeline component
	itemTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Item Timeline"));

}

// Called when the game starts or when spawned
void AInspectableItem::BeginPlay()
{
	Super::BeginPlay();

	//start position is the current object location
	startPosition = this->GetActorLocation();

	//bind timeline update function
	updateTimelineFloat.BindDynamic(this, &AInspectableItem::UpdateTimelineComp);

	if (itemCurve) //if the item curve has been set
	{
		//add the float track to the timeline
		itemTimeline->AddInterpFloat(itemCurve, updateTimelineFloat);
	}

}

// Called every frame
void AInspectableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AInspectableItem::SetItemTransform(FVector positionToSet, FRotator rotationToSet)
{
	//set the target position and rotation
	targetPositon = positionToSet;
	targetRotation = rotationToSet;
	itemTimeline->Play(); //play the timeline which will use those values
}


void AInspectableItem::UpdateTimelineComp(float output)
{
	//lerp smoothly between the start and target transform values based on the output of the timeline
	this->SetActorLocationAndRotation(FMath::Lerp(startPosition, targetPositon, output), FMath::Lerp(startRotation, targetRotation, output));
}

