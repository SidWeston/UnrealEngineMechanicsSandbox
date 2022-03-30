// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "InteractButtonComp.h"
#include "Kismet/GameplayStatics.h"
#include "ButtonLift.generated.h"


UCLASS()
class UE4MECHANICSSANDBOX_API AButtonLift : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AButtonLift();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//static mesh components
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* liftBase;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* liftMesh;

	//lift trigger
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* liftTrigger;

	//timeline component
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* liftTimeline;

	//vectors for the start and end position of the lift
	UPROPERTY(VisibleAnywhere)
	FVector liftStartPos;
	UPROPERTY(VisibleAnywhere, meta = (MakeEditWidget = true)) //make the end position manipulatable in the editor with a widget/moveable point in the world
	FVector liftEndPos;

	//button component
	UPROPERTY(VisibleAnywhere)
	UInteractButtonComp* liftButton;

	bool playerOnLift; //is the player on the lift
	bool isLiftUp; //is the lift at the top

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UCurveFloat* liftCurve; //timeline curve

	FOnTimelineFloat UpdateTimelineFloat;

	FOnTimelineEvent TimelineFinishedEvent; //for when the timeline finishes

	UFUNCTION()
	void UpdateTimelineComp(float output); //update timeline function

	UFUNCTION()
	void TimelineFinished(); //function for when the timeline finishes

	//overlap functions
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
