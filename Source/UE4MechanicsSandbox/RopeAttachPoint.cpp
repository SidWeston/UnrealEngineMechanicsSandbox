// Fill out your copyright notice in the Description page of Project Settings.


#include "RopeAttachPoint.h"

// Sets default values
ARopeAttachPoint::ARopeAttachPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create mesh components
	attachPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Attach Point"));
	attachPoint->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

}

// Called when the game starts or when spawned
void ARopeAttachPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARopeAttachPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

