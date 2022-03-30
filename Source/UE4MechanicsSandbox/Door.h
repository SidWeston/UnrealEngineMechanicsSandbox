// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"
#include "Door.generated.h"

UCLASS()
class UE4MECHANICSSANDBOX_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//mesh components
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* doorMesh;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* doorFrame;
	//door trigger
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* boxTrigger;

	//rotators for the positive, negative and target rotations
	UPROPERTY(EditAnywhere)
	FRotator targetRotation; //target rotation is where the door will open to, this will be set to either the positive or negative rotation depending on the direction the player is coming from
	UPROPERTY(VisibleAnywhere)
	FRotator positiveRotation;
	UPROPERTY(VisibleAnywhere)
	FRotator negativeRotation;

	//timeline component
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* doorTimeline;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UCurveFloat* doorCurve; //timeline curve

	FOnTimelineFloat UpdateTimelineFloat;

	UFUNCTION()
	void UpdateTimelineComp(float output); //timeline update function

	//door begin and end overlap function definitions
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



};
