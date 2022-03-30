// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "WeightedButtonDoor.generated.h"

UCLASS()
class UE4MECHANICSSANDBOX_API AWeightedButtonDoor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AWeightedButtonDoor();

	//mesh components
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* doorFrame;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* doorMesh;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* weightedButton;

	//trigger component
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* buttonTrigger;

	//timeline component
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* doorTimeline;

	//if the player is on the button or an object is
	bool playerOnButton;
	bool objectOnButton;

	//vectors for the door to open and close
	UPROPERTY(VisibleAnywhere)
	FVector closedLocation;
	UPROPERTY(EditAnywhere)
	FVector openLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UCurveFloat* doorCurve; //timeline curve

	FOnTimelineFloat updateTimelineFloat;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void UpdateTimelineComp(float output); //timeline update function

	//trigger overlap functions
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
