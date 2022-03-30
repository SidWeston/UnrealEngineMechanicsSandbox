// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"
#include "Lift.generated.h"

UCLASS()
class UE4MECHANICSSANDBOX_API ALift : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALift();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//mesh components
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* liftMesh;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* liftBack;

	//trigger component
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* liftTrigger;

	//timeline component
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* liftTimeline;

	//vectors for the start and end pos of the lift
	UPROPERTY(VisibleAnywhere)
	FVector liftStartPos;
	UPROPERTY(VisibleAnywhere, meta = (MakeEditWidget = true)) //end pos makes an edit widget so the end location is editable in the scene view
	FVector liftEndPos;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UCurveFloat* liftCurve; //timeline curve

	FOnTimelineFloat UpdateTimelineFloat;

	UFUNCTION()
	void UpdateTimelineComp(float output); //timeline update function

	//trigger overlap functions
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
