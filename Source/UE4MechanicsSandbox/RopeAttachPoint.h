// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RopeAttachPoint.generated.h"

UCLASS()
class UE4MECHANICSSANDBOX_API ARopeAttachPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARopeAttachPoint();

	//actor with an empty static mesh component.
	//this actors purpose is to serve as a location for the player's grappling hook to attach to 
	UStaticMeshComponent* attachPoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
