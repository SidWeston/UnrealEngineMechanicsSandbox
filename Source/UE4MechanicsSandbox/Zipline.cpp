// Fill out your copyright notice in the Description page of Project Settings.


#include "Zipline.h"
#include "kismet/GameplayStatics.h"
#include "DualViewCharacterController.h"
#include "GameFramework/Character.h"

// Sets default values
AZipline::AZipline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//create mesh components
	ziplineAnchorPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Zipline Anchor"));
	ziplineAnchorPoint->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	ziplineStart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Zipline Start"));
	ziplineStart->AttachToComponent(ziplineAnchorPoint, FAttachmentTransformRules::KeepWorldTransform);
	ziplineEnd = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Zipline end"));
	ziplineEnd->AttachToComponent(ziplineAnchorPoint, FAttachmentTransformRules::KeepWorldTransform);

	//create cable component
	ziplineCable = CreateDefaultSubobject<UCableComponent>(TEXT("Zipline Cable"));
	ziplineCable->AttachToComponent(ziplineStart, FAttachmentTransformRules::KeepWorldTransform);
	//attach the end of the zipline cable to the end pole of the zipline
	ziplineCable->SetAttachEndToComponent(ziplineEnd, "None");
	//ensure the zipline cable is visible in the game
	ziplineCable->SetHiddenInGame(false);

	//create interaction component
	ziplineInteraction = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));
	//bind interact function
	ziplineInteraction->FInteract.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(AZipline, OnInteract));

	//create timeline
	ziplineTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Zipline Timeline"));

}

// Called when the game starts or when spawned
void AZipline::BeginPlay()
{
	Super::BeginPlay();
	//bind timeline function
	updateTimelineFloat.BindDynamic(this, &AZipline::UpdateTimelineComp);

	if (ziplineCurve) //if the zipline curve is set
	{
		//add a float track
		ziplineTimeline->AddInterpFloat(ziplineCurve, updateTimelineFloat);
	}

	//get a reference to the player character. assumes the player is using the dual view character controller
	playerCharacter = Cast<ADualViewCharacterController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}

// Called every frame
void AZipline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(playerCharacter == nullptr) //if the player character is not set, will attempt to find the player character again
	{
		playerCharacter = Cast<ADualViewCharacterController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}

	if(ziplineTimeline->IsPlaying() && playerCharacter->hasJumped) //if the timeline is playing and the character jumps
	{
		ziplineTimeline->Stop(); //stops playing the zipline
	}

}

void AZipline::OnInteract()
{
	//finds out which end of the zipline got interacted with
	if(ziplineInteraction->interactedComponent == ziplineStart) //if the start was interacted with
	{
		ziplineTimeline->PlayFromStart(); //plays from the start
	}
	else if(ziplineInteraction->interactedComponent == ziplineEnd) //if the end got interacted with
	{
		ziplineTimeline->ReverseFromEnd(); //play from the end
	}

}

void AZipline::UpdateTimelineComp(float output)
{
	//lerp the player positon between the two zipline poles based on the output of the timeline
	playerCharacter->SetActorLocation(FMath::Lerp(ziplineStart->GetComponentLocation(), ziplineEnd->GetComponentLocation(), output));
}

