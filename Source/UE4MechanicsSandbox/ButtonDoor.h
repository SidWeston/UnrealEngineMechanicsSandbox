// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractButtonComp.h"
#include "Components/TimelineComponent.h"
#include "ButtonDoor.generated.h"

UCLASS()
class UE4MECHANICSSANDBOX_API AButtonDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AButtonDoor();

	//mesh components
	UPROPERTY(VisibleAnywhere)
	UInteractButtonComp* doorButton;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* doorFrame;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* doorMesh;

	//open and closed vectors
	UPROPERTY(EditAnywhere)
	FVector openLocation;
	UPROPERTY(VisibleAnywhere)
	FVector closedLocation;

	//timeline component
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* doorTimeline;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UCurveFloat* doorCurve; //timeline curve
	FOnTimelineFloat updateTimelineFloat;

	//timeline update function
	UFUNCTION()
	void UpdateTimelineComp(float output);

};
