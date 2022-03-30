// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionComponent.h"
#include "InteractButton.generated.h"

UCLASS()
class UE4MECHANICSSANDBOX_API AInteractButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractButton();

	//mesh components
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* buttonBackground;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* buttonObject;

	//has the button been pressed
	UPROPERTY(VisibleAnywhere)
	bool buttonPressed; 

	//interaction component
	UPROPERTY(VisibleAnywhere)
	UInteractionComponent* interactionComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//function called when the button is interacted with
	UFUNCTION()
	void OnButtonPressed();

};
