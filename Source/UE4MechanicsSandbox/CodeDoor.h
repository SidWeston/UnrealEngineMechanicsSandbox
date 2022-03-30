// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CodeDoor.generated.h"


UCLASS()
class UE4MECHANICSSANDBOX_API ACodeDoor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ACodeDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//materials showing whether the door is locked or unlocked
	UPROPERTY(EditAnywhere)
	UMaterial* lockedMaterial;
	UPROPERTY(EditAnywhere)
	UMaterial* unlockedMaterial;

	//static mesh components
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* doorFrame;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* doorMesh;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* keypadBack;

	//vectors for the closed and open positions of the door mesh
	//open location should be editable within the editor to make it easier to configure
	UPROPERTY(EditAnywhere) 
	FVector openLocation; 
	FVector closedLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString doorCode; //the code that is required to be entered for the door to open

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool playerInRange; //is the player in range of the door for it to open

	UPROPERTY()
	bool doorUnlocked; //is the door unlocked from the keypad

	//box trigger
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* doorTrigger; 

	//timeline component
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* doorTimeline;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//the curve float is the actual editable timeline/curve that is edited to determine how long the timeline plays for etc
	UPROPERTY(EditAnywhere)
	UCurveFloat* doorCurve;

	FOnTimelineFloat updateTimelineFloat;

	UFUNCTION()
	void UpdateTimelineComp(float output);  //timeline update function

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) //allows the widget component to be referenced as a variable within the blueprint editor
	UWidgetComponent* keyPadWidget;

	UFUNCTION(BlueprintCallable) //allows the function to be called within the blueprint editor
	void CheckCode(FString CodeEntered);

	//overlap functions for the box trigger
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
