// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaceTracker.h"
#include "GameFramework/Actor.h"
#include "TrackerTrigger.generated.h"

UCLASS()
class ARCADERACER_API ATrackerTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrackerTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	ARaceTracker* raceTracker;
	UPROPERTY(VisibleAnywhere)
	int triggerIndex;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetRaceTracker(ARaceTracker* tracker);
	void SetTriggerIndex(int index);

	UFUNCTION(BlueprintCallable)
	void PlayerEnter(AActor* inPlayer);
};
