// Fill out your copyright notice in the Description page of Project Settings.


#include "Lift.h"

#include "GameFramework/Character.h"

// Sets default values
ALift::ALift()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create mesh components
	//lift back is empty, and is just used as the root component so there are no issues with scaling
	liftBack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lift Back"));
	liftBack->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	liftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lift Mesh"));
	liftMesh->AttachToComponent(liftBack, FAttachmentTransformRules::KeepWorldTransform);

	//create trigger component
	liftTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Lift Trigger"));
	liftTrigger->AttachToComponent(liftMesh, FAttachmentTransformRules::KeepWorldTransform);
	//bind overlap functions to trigger
	liftTrigger->OnComponentBeginOverlap.AddDynamic(this, &ALift::OnBeginOverlap);
	liftTrigger->OnComponentEndOverlap.AddDynamic(this, &ALift::OnEndOverlap);

	//create timeline
	liftTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Lift Timeline"));

}

// Called when the game starts or when spawned
void ALift::BeginPlay()
{
	Super::BeginPlay();
	//set the start pos to be the current actor location, assumes the lift will always start down
	liftStartPos = GetActorLocation();
	liftEndPos += GetActorLocation(); //conver the end pos into world space

	//bind the timeline update function
	UpdateTimelineFloat.BindDynamic(this, &ALift::UpdateTimelineComp);

	if(liftCurve) //if the lift curve has been set
	{
		//add the float track to the timeline
		liftTimeline->AddInterpFloat(liftCurve, UpdateTimelineFloat);
	}

}

// Called every frame
void ALift::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALift::UpdateTimelineComp(float output)
{
	//liftMesh->SetRelativeLocation(FMath::Lerp(liftStartPos, liftEndPos, output));
	liftMesh->SetWorldLocation(FMath::Lerp(liftStartPos, liftEndPos, output));
}

void ALift::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0); //get a reference to the player
	if(playerCharacter->GetUniqueID() == OtherActor->GetUniqueID()) //if the player is the overlapping actor
	{
		liftTimeline->Play(); //play timeline
	}
}

void ALift::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0); //get a reference to the player
	if (playerCharacter->GetUniqueID() == OtherActor->GetUniqueID()) //if the player is the overlapping actor
	{
		liftTimeline->Reverse(); //reverse timeline
	}
}


