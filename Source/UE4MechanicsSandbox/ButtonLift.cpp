// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonLift.h"

#include "GameFramework/Character.h"

// Sets default values
AButtonLift::AButtonLift()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create mesh components
	liftBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lift Base"));
	liftBase->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	liftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lift Mesh"));
	liftMesh->AttachToComponent(liftBase, FAttachmentTransformRules::KeepWorldTransform);

	//create trigger component
	liftTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Lift Trigger"));
	liftTrigger->AttachToComponent(liftMesh, FAttachmentTransformRules::KeepWorldTransform);

	//bind overlap functions to trigger
	liftTrigger->OnComponentBeginOverlap.AddDynamic(this, &AButtonLift::OnBeginOverlap);
	liftTrigger->OnComponentEndOverlap.AddDynamic(this, &AButtonLift::OnEndOverlap);

	//create button component
	liftButton = CreateDefaultSubobject<UInteractButtonComp>(TEXT("Lift Button"));
	liftButton->buttonObject->AttachToComponent(liftMesh, FAttachmentTransformRules::KeepWorldTransform);

	//create timeline component
	liftTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Lift Timeline"));

	//assumes the lift starts in the down position
	isLiftUp = false;
}

// Called when the game starts or when spawned
void AButtonLift::BeginPlay()
{
	Super::BeginPlay();

	//the start pos should be the lift position at the start of the game/playthrough
	liftStartPos = GetActorLocation();
	liftEndPos += GetActorLocation(); //convert the end position to world space by adding the current actor location to it

	//bind timeline events
	UpdateTimelineFloat.BindDynamic(this, &AButtonLift::UpdateTimelineComp);
	TimelineFinishedEvent.BindDynamic(this, &AButtonLift::TimelineFinished);

	if (liftCurve) //if the timeline curve has been set in the editor
	{
		//bind events
		liftTimeline->AddInterpFloat(liftCurve, UpdateTimelineFloat);
		liftTimeline->SetTimelineFinishedFunc(TimelineFinishedEvent);
	}

}

// Called every frame
void AButtonLift::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//when the button is pressed it changes the value of buttonPressed between true and false, 
	if(liftButton->buttonPressed && playerOnLift && !isLiftUp) //true means the lift will move up if it isnt already up,
	{
		liftTimeline->Play();
	}
	else if(!liftButton->buttonPressed && playerOnLift && isLiftUp) //false means the lift will move down if it isnt already down.
	{
		liftTimeline->Reverse();
	}

}

void AButtonLift::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);//get a reference to the player character in the game world
	if(playerCharacter->GetUniqueID() == OtherActor->GetUniqueID()) //if the player character's id matches the id of the actor that has overlapped with the lift, 
	{
		playerOnLift = true; //the player will be recognised as on the lift
	}
}

void AButtonLift::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0); //get a reference to the player character
	if (playerCharacter->GetUniqueID() == OtherActor->GetUniqueID()) //compare with the actor that just stopped overlapping
	{
		playerOnLift = false;
	}
}

void AButtonLift::UpdateTimelineComp(float output)
{
	//lerp between the start and the end position of the lift based on the position of the timeline
	liftMesh->SetWorldLocation(FMath::Lerp(liftStartPos, liftEndPos, output));
}

void AButtonLift::TimelineFinished() //when the timeline if finished
{
	//this assumes the start position of the lift is on the ground
	if(liftMesh->GetComponentLocation() == liftStartPos) //if the lift is at the start position
	{
		isLiftUp = false; //the lift is not up
	}
	else if(liftMesh->GetComponentLocation() == liftEndPos) //if the lift is at the end position
	{
		isLiftUp = true; //the lift is up
	}
}




