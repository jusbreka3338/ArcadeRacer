// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackerTrigger.h"

// Sets default values
ATrackerTrigger::ATrackerTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrackerTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrackerTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrackerTrigger::SetRaceTracker(ARaceTracker* tracker)
{
	raceTracker = tracker;
}

void ATrackerTrigger::SetTriggerIndex(int index)
{
	triggerIndex = index;
}

void ATrackerTrigger::PlayerEnter(AActor* inPlayer)
{
	raceTracker->PlayerEnterTrigger(inPlayer, triggerIndex);
}



