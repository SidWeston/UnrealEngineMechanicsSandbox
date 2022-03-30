// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionComponent.h"
#include "InteractTestObject.generated.h"

///
/// NOT USED IN THE PROJECT
///	THIS WAS CREATED FOR TESTING PURPOSES ONLY WHEN FIRST CREATING THE INTERACTION COMPONENT
///


UCLASS()
class UE4MECHANICSSANDBOX_API AInteractTestObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractTestObject();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* objMesh;

	UPROPERTY(VisibleAnywhere)
	UInteractionComponent* InteractionComponent;

	UFUNCTION()
	void InteractWithObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
