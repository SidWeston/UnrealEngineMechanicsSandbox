// Fill out your copyright notice in the Description page of Project Settings.


#include "LiftV2.h"

#include "GameFramework/Character.h"

// Sets default values
ALiftV2::ALiftV2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create mesh components
	liftBack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lift Back"));
	liftBack->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	liftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lift Mesh"));
	liftMesh->AttachToComponent(liftBack, FAttachmentTransformRules::KeepWorldTransform);

	//create trigger component
	liftTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Lift Trigger"));
	liftTrigger->AttachToComponent(liftMesh, FAttachmentTransformRules::KeepWorldTransform);
	//bind overlap functions to trigger
	liftTrigger->OnComponentBeginOverlap.AddDynamic(this, &ALiftV2::OnBeginOverlap);
	liftTrigger->OnComponentEndOverlap.AddDynamic(this, &ALiftV2::OnEndOverlap);

	//create timeline component
	liftTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Lift Timeline"));

}

// Called when the game starts or when spawned
void ALiftV2::BeginPlay()
{
	Super::BeginPlay();

	//set start and end pos
	liftStartPos = GetActorLocation();
	liftEndPos += GetActorLocation(); //converts end pos to world space

	//bind timeline events
	UpdateTimelineFloat.BindDynamic(this, &ALiftV2::UpdateTimelineComp);
	TimelineFinishedEvent.BindDynamic(this, &ALiftV2::TimelineFinished);

	if (liftCurve) //if the lift curve has been set 
	{
		//add float track
		liftTimeline->AddInterpFloat(liftCurve, UpdateTimelineFloat);
		//set the timeline finished event
		liftTimeline->SetTimelineFinishedFunc(TimelineFinishedEvent);
	}

}

// Called every frame
void ALiftV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALiftV2::UpdateTimelineComp(float output)
{
	//lerp between the start and the end pos based on the output of the timeline
	liftMesh->SetWorldLocation(FMath::Lerp(liftStartPos, liftEndPos, output));
}

void ALiftV2::TimelineFinished()
{
	//swaps between if the lift is up or down
	if(isLiftUp)
	{
		isLiftUp = false;
	}
	else if(!isLiftUp)
	{
		isLiftUp = true;
	}
}

void ALiftV2::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0); //get a reference to the player
	if (playerCharacter->GetUniqueID() == OtherActor->GetUniqueID()) //if the player is the overlapping actor
	{
		if(isLiftUp) //move the lift down if it is at the top
		{
			liftTimeline->Reverse();
		}
		else if(!isLiftUp) //move the lift up if it is at the bottom
		{
			liftTimeline->Play();
		}
	}
}

void ALiftV2::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//REDUNDANT
	//const ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//if (playerCharacter->GetUniqueID() == OtherActor->GetUniqueID())
	//{
		//if(liftTimeline->IsPlaying())
		//{
		//	liftTimeline->Reverse();
		//}
		//else if(liftTimeline->IsReversing())
		//{
		//	liftTimeline->Play();
		//}
	//}
}


