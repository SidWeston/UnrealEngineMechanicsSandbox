// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "InteractDoor.generated.h"

UCLASS()
class UE4MECHANICSSANDBOX_API AInteractDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractDoor();

	//mesh components
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* doorFrame;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* doorMesh;

	//door trigger
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* doorTrigger;

	//interaction component
	UPROPERTY(VisibleAnywhere)
	UInteractionComponent* interactionComponent;

	//timeline component
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* doorTimeline;

	//is the player in range of the door
	bool playerInRange;

	//rotator values for the target, positive, negative and closed rotation
	UPROPERTY(EditAnywhere)
	FRotator targetRotation;
	UPROPERTY(VisibleAnywhere)
	FRotator positiveRotation;
	UPROPERTY(VisibleAnywhere)
	FRotator negativeRotation;
	FRotator closedRotation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UCurveFloat* doorCurve; //timeline curve

	FOnTimelineFloat updateTimelineFloat;

	//update timeline function
	UFUNCTION()
	void UpdateTimelineComp(float output);

	//interact function
	UFUNCTION()
	void InteractWithDoor();

	//trigger overlap functions
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
