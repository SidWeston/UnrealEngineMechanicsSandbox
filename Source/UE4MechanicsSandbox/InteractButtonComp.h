// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.h"
#include "InteractButtonComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4MECHANICSSANDBOX_API UInteractButtonComp : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UInteractButtonComp();

	//create mesh component
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* buttonObject;

	//has the button been pressed
	UPROPERTY(VisibleAnywhere)
	bool buttonPressed;

	//create interaction component
	UPROPERTY(VisibleAnywhere)
	UInteractionComponent* interactionComponent;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//button pressed function
	UFUNCTION()
	void OnButtonPressed();
		
};
