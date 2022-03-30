// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "InspectableItem.generated.h"

UCLASS()
class UE4MECHANICSSANDBOX_API AInspectableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInspectableItem();
	//mesh component of the object
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* itemMesh;

	//item information, readable in blueprints to be used in UI 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString itemName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString itemDescription;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//start and targets for position and rotation of the object
	FVector startPosition;
	FVector targetPositon;
	FRotator startRotation;
	FRotator targetRotation;

	//set the transform of the item, called when object interacted with
	void SetItemTransform(FVector positionToSet, FRotator rotationToSet);

	//timeline component to make a smooth animation
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* itemTimeline;

	//timeline curve
	UPROPERTY(EditAnywhere)
	UCurveFloat* itemCurve;

	FOnTimelineFloat updateTimelineFloat;

	//timeline update function
	UFUNCTION()
	void UpdateTimelineComp(float output);

};
