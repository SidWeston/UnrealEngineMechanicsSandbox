// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchPad.h"
#include "DualViewCharacterController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ALaunchPad::ALaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create launch pad mesh
	launchPadBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Launch Pad Base"));
	launchPadBody->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	//create launch pad trigger
	launchPadTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Launch Pad Trigger"));
	launchPadTrigger->AttachToComponent(launchPadBody, FAttachmentTransformRules::KeepWorldTransform);
	//bind overlap events to c++ functions
	launchPadTrigger->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::OnBoxBeginOverlap);
	//default launch velocity value, can be changed in editor
	launchVelocity = 500.0f; 

}

// Called when the game starts or when spawned
void ALaunchPad::BeginPlay()
{
	Super::BeginPlay();

	//get a reference to the player character through casting
	playerCharacter = Cast<ADualViewCharacterController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void ALaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(playerCharacter == nullptr) //if the player character is null, try to find the player by casting again
	{
		playerCharacter = Cast<ADualViewCharacterController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
}


void ALaunchPad::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->GetUniqueID() == playerCharacter->GetUniqueID()) //if the player is the actor overlapping
	{
		//calculate the direction to launch the player
		FVector launchDirection = playerCharacter->GetActorUpVector() * launchVelocity;
		//launch the player, overriding the both axes
		playerCharacter->LaunchCharacter(launchDirection, true, true);
	}
}




