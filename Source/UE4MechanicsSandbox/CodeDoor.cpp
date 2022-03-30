// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeDoor.h"

#include "GameFramework/Character.h"

// Sets default values
ACodeDoor::ACodeDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create mesh components
	doorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Frame"));
	doorFrame->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	doorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	doorMesh->AttachToComponent(doorFrame, FAttachmentTransformRules::KeepRelativeTransform);

	keypadBack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Keypad Back"));
	keypadBack->AttachToComponent(doorFrame, FAttachmentTransformRules::KeepRelativeTransform);

	//create widget component
	keyPadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Key Pad"));
	keyPadWidget->AttachToComponent(doorFrame, FAttachmentTransformRules::KeepRelativeTransform);

	//create door trigger component
	doorTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Door Trigger"));
	doorTrigger->AttachToComponent(doorFrame, FAttachmentTransformRules::KeepRelativeTransform);

	//bind overlap functions to door trigger
	doorTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACodeDoor::OnBoxBeginOverlap);
	doorTrigger->OnComponentEndOverlap.AddDynamic(this, &ACodeDoor::OnBoxEndOverlap);

	//create timeline component
	doorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Door Timeline"));

	//door starts locked
	doorUnlocked = false;
}

// Called when the game starts or when spawned
void ACodeDoor::BeginPlay()
{
	Super::BeginPlay();
	//set the closed location to be the current location of the door at game start
	closedLocation = doorMesh->GetRelativeLocation();
	//set the keypad material to be locked initially
	keypadBack->SetMaterial(0, lockedMaterial);

	//bind timeline to function
	updateTimelineFloat.BindDynamic(this, &ACodeDoor::UpdateTimelineComp);

	if(doorCurve) //if the door timeline has been set in the editor
	{
		//add the float track to the timeline
		doorTimeline->AddInterpFloat(doorCurve, updateTimelineFloat);
	}
}

// Called every frame
void ACodeDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (playerInRange && doorUnlocked) //if the player is within the proximity of the door and the door is unlocked
	{
		doorTimeline->Play();
	}
	else if (!playerInRange || !doorUnlocked) //if either condition is false, it will close the door
	{
		doorTimeline->Reverse();
	}

}

void ACodeDoor::UpdateTimelineComp(float output)
{
	//lerp between the closed and open locations of the door to create a smooth opening animation based on the output of the timeline
	doorMesh->SetRelativeLocation(FMath::Lerp(closedLocation, openLocation, output));
}


void ACodeDoor::CheckCode(FString CodeEntered)
{
	if (CodeEntered == doorCode) //if the code is correct
	{
		doorUnlocked = true; //unlocks the door
		keypadBack->SetMaterial(0, unlockedMaterial); //set the material of the door to signify the door is unlocked
	}
	else if(CodeEntered != doorCode) //if the code is incorrect
	{
		doorUnlocked = false; //locks the door
		keypadBack->SetMaterial(0, lockedMaterial); //sets the material to be locked
	}
}

void ACodeDoor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//find a reference to the player character currently being used in the scene
	const ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if(OtherActor->GetUniqueID() == playerCharacter->GetUniqueID()) //if the object overlapping with the trigger is the same as the player character
	{
		playerInRange = true; //allows the door to be opened
	}
}

void ACodeDoor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0); //player character ref
	if (OtherActor->GetUniqueID() == playerCharacter->GetUniqueID()) //checks if it matches the overlapping actor
	{
		playerInRange = false; //stops the door from being opened
	}
}
