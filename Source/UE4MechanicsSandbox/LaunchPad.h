// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "LaunchPad.generated.h"

//forward declare
class ADualViewCharacterController;

UCLASS()
class UE4MECHANICSSANDBOX_API ALaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaunchPad();

	//launch pad mesh
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* launchPadBody;

	//trigger component
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* launchPadTrigger;

	//how far/high the player should be launched
	UPROPERTY(EditAnywhere)
	float launchVelocity;

	//reference to the player character, assumes the player is using this specific character controller
	ADualViewCharacterController* playerCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//trigger overlap functions
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
