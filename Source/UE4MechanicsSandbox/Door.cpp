// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "GameFramework/Character.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create static mesh components
	doorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Frame"));
	doorFrame->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	doorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	doorMesh->AttachToComponent(doorFrame, FAttachmentTransformRules::KeepWorldTransform);

	//create door trigger
	boxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Door Trigger"));
	boxTrigger->AttachToComponent(doorFrame, FAttachmentTransformRules::KeepWorldTransform);
	//bind overlap functions to door trigger
	boxTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnBoxBeginOverlap);
	boxTrigger->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnBoxEndOverlap);

	//create timeline component
	doorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Door Timeline"));

	//set positive and negative rotation
	positiveRotation = FRotator(0, 90, 0);
	negativeRotation = FRotator(0, -90, 0);

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	//bind timeline update functions
	UpdateTimelineFloat.BindDynamic(this, &ADoor::UpdateTimelineComp);

	if(doorCurve) //if the timeline curve has been set
	{
		doorTimeline->AddInterpFloat(doorCurve, UpdateTimelineFloat);
	}

}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	const ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0); //get a reference to the player character in the world
	if (playerCharacter->GetUniqueID() == OtherActor->GetUniqueID()) //if the player character is the same actor that began the overlap
	{
		//get the dot product of the player forward vector and the door forward vector to determine which direction the door should open
		float isFacing = FVector::DotProduct(playerCharacter->GetActorForwardVector(), this->GetActorForwardVector()); //this means the door will always open away from the direction the player is facing
		if(isFacing >= 0)
		{
			targetRotation = positiveRotation;
		}
		else if(isFacing < 0)
		{
			targetRotation = negativeRotation;
		}
		doorTimeline->Play(); //play the timeline
	}
}

void ADoor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0); //get a reference to the player character
	if (playerCharacter->GetUniqueID() == OtherActor->GetUniqueID()) //if the player character was the actor that ended the overlap
	{
		doorTimeline->Reverse(); //reverse the timeline
	}
}

void ADoor::UpdateTimelineComp(float output)
{
	//Set the rotation of the door using a lerp to create a smooth animation based on the output of the timeline update function
	doorMesh->SetRelativeRotation(FMath::Lerp(this->GetActorRotation(), targetRotation, output));
}



