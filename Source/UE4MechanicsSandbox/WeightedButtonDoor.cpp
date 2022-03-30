// Fill out your copyright notice in the Description page of Project Settings.


#include "WeightedButtonDoor.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeightedButtonDoor::AWeightedButtonDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create mesh components
	doorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Frame"));
	doorFrame->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	doorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Mesh"));
	doorMesh->AttachToComponent(doorFrame, FAttachmentTransformRules::KeepWorldTransform);
	weightedButton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weighted Button"));
	weightedButton->AttachToComponent(doorFrame, FAttachmentTransformRules::KeepWorldTransform);

	//create trigger component
	buttonTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Button Trigger"));
	buttonTrigger->AttachToComponent(weightedButton, FAttachmentTransformRules::KeepWorldTransform);
	//bind trigger overlap functions
	buttonTrigger->OnComponentBeginOverlap.AddDynamic(this, &AWeightedButtonDoor::OnBoxBeginOverlap);
	buttonTrigger->OnComponentEndOverlap.AddDynamic(this, &AWeightedButtonDoor::OnBoxEndOverlap);

	//create timeline component
	doorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Door Timeline"));

}

// Called when the game starts or when spawned
void AWeightedButtonDoor::BeginPlay()
{
	Super::BeginPlay();
	//set the closed location of the door. assumes the door starts closed
	closedLocation = doorMesh->GetRelativeLocation();

	updateTimelineFloat.BindDynamic(this, &AWeightedButtonDoor::UpdateTimelineComp);

	if (doorCurve) //if the door curve has been set
	{
		//add a float track to the timeline
		doorTimeline->AddInterpFloat(doorCurve, updateTimelineFloat);
	}

}

// Called every frame
void AWeightedButtonDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeightedButtonDoor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if(OtherActor->ActorHasTag("Player")) //if the player is overlapping with the button
	{
		playerOnButton = true;
		doorTimeline->Play();
	}
	else if(OtherActor->ActorHasTag("Pickup")) //if an object is overlapping with the button
	{
		objectOnButton = true;
		doorTimeline->Play();
	}
}

void AWeightedButtonDoor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->ActorHasTag("Player")) //if the player left the button
	{
		playerOnButton = false;
		if(!objectOnButton) //and there is no object on the button
		{
			doorTimeline->Reverse(); //close the door
		}
	}
	else if(OtherActor->ActorHasTag("Pickup")) //if an object left the button
	{
		objectOnButton = false;
		if(!playerOnButton) //and there is no player on the button
		{
			doorTimeline->Reverse(); //close the door
		}
	}
}

void AWeightedButtonDoor::UpdateTimelineComp(float output)
{
	//lerp between the closed and open locations based on the output of the timeline
	doorMesh->SetRelativeLocation(FMath::Lerp(closedLocation, openLocation, output));
}

