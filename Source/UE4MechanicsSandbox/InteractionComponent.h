// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

DECLARE_DELEGATE(FInteractionDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4MECHANICSSANDBOX_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//need two different activation functions based on if it is an actor or a component that was interacted with
	virtual void ActivationFunction(AActor* actorInteractedWith);
	virtual void ActivationFunction(UActorComponent* componentInteractedWith);

	//reference to the actor or actor components that have been interacted with
	AActor* interactedActor; //e.g an entire door
	UActorComponent* interactedComponent; //e.g button

	//creates a delegate to call interact functions on objects with this component
	FInteractionDelegate FInteract;
};
