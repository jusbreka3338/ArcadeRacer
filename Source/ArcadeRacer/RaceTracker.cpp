// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceTracker.h"

#include "StringHelper.h"
#include "TrackerTrigger.h"
#include "Containers/Array.h"

// Sets default values
ARaceTracker::ARaceTracker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UClass> bpClassFinder(TEXT("Class'/Game/Blueprints/BP_TrackerTrigger.BP_TrackerTrigger_C'"));
	if (bpClassFinder.Object)
	{
		triggerClass = bpClassFinder.Object;
	}
}

// Called when the game starts or when spawned
void ARaceTracker::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnTriggers();
}

// Called every frame
void ARaceTracker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleTracking();
}

void ARaceTracker::SpawnTriggers()
{
	if (trackSplineHandler == nullptr)
	{
		StringHelper::Print("Spline handler null");
		return;
	}
	USplineComponent* spline = trackSplineHandler->GetComponentByClass<USplineComponent>();
	
	if (triggerClass == nullptr)
	{
		StringHelper::Print("Trigger null");
		return;
	}
	
	for (int i = 0; i < spline->GetNumberOfSplinePoints(); i += 1)
	{
		ATrackerTrigger* newTrigger = static_cast<ATrackerTrigger*>(GetWorld()->SpawnActor(triggerClass));
		newTrigger->SetActorLocation(spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
		newTrigger->SetActorRotation(spline->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::World));

		newTrigger->SetRaceTracker(this);
		newTrigger->SetTriggerIndex(i);
		triggers.Add(newTrigger);

		players.Add(FActorEncapsulate());
	}
}

void ARaceTracker::PlayerEnterTrigger(AActor* inPlayer, int triggerIndex)
{
	if (!playerActors.Contains(inPlayer))
	{
		playerActors.Add(inPlayer);
		playerLaps.Add(0);
		playerPassedTriggers.Add(0);
	}

	int oldPlayerPos = triggers.Find(RequestLastTrigger(inPlayer));
	if (players[triggerIndex].list.Contains(inPlayer)) return;
	for (int i = 0; i < players.Num(); i += 1)
	{
		players[i].list.Remove(inPlayer);
	}
	players[triggerIndex].list.Add(inPlayer);

	int playerIndex = playerActors.Find(inPlayer);
	if (oldPlayerPos < triggerIndex)
	{
		playerPassedTriggers[playerIndex] += 1;
	}
	else if (triggerIndex == 0)
	{
		if (playerPassedTriggers[playerIndex] >= triggers.Num() - 1)
		{
			playerLaps[playerIndex] += 1; // Lap Player
			playerPassedTriggers[playerIndex] = 0;
		}
		if (playerLaps[playerIndex] >= lapRequirement) finishedPlayers.Add(inPlayer);
		if (finishedPlayers.Num() >= playerActors.Num()) raceFinished = true;
	}
}

void ARaceTracker::HandleTracking()
{
	TArray<FActorEncapsulate> copiedPlayers;
	for (FActorEncapsulate encap : players) copiedPlayers.Add(encap);
	
	TArray<AActor*> laplessSortedPlayers;
	for (int i = copiedPlayers.Num() - 1; i > -1; i -= 1)
	{
		TArray<AActor*> localSortedPlayers;
		for (AActor* player : copiedPlayers[i].list)
		{
			if (localSortedPlayers.Num() == 0)
			{
				localSortedPlayers.Add(player);
				continue;
			}

			float dist = player->GetDistanceTo(triggers[i]);
			bool playerPlaced = false;
			for (int j = 0; j < localSortedPlayers.Num(); j += 1)
			{
				float compareDist = FVector::Distance(localSortedPlayers[j]->GetActorLocation(), triggers[i]->GetActorLocation());
				if (dist < compareDist)
				{
					localSortedPlayers.Insert(player, j);
					playerPlaced = true;
					break;
				}

			}
			
			if (playerPlaced) continue;
			localSortedPlayers.Add(player);
		}

		const int localPlayerCount = localSortedPlayers.Num();
		for (int j = 0; j < localPlayerCount; j += 1)
		{
			laplessSortedPlayers.Add(localSortedPlayers[localPlayerCount - (j + 1)]);
		}
	}

	int maxLap = 0;
	for (int lap : playerLaps) if (lap > maxLap) maxLap = lap; // Get highest lap count
	TArray<FActorEncapsulate> lapSortedPlayers;
	for (int i = 0; i < maxLap + 1; i += 1) lapSortedPlayers.Add(FActorEncapsulate());
	for (AActor* player : laplessSortedPlayers)
	{
		lapSortedPlayers[playerLaps[playerActors.Find(player)]].list.Add(player); // Place players into 2d array where row is lap count
	}
	
	sortedPlayers.Empty();
	for (int lapIndex = lapSortedPlayers.Num() - 1; lapIndex > -1; lapIndex -= 1)
	{
		for (AActor* player : lapSortedPlayers[lapIndex].list) sortedPlayers.Add(player);
	}
}


int ARaceTracker::RequestPosition(AActor* player)
{
	if (!sortedPlayers.Contains(player)) return -1;

	return sortedPlayers.Find(player);
}

int ARaceTracker::RequestLap(AActor* player)
{
	if (!playerActors.Contains(player)) return 0;

	return playerLaps[playerActors.Find(player)];
}
	

AActor* ARaceTracker::RequestLastTrigger(AActor* player)
{
	TArray<FActorEncapsulate> copiedPlayers = players;
	for (int i = 0; i < copiedPlayers.Num(); i += 1)
	{
		if (copiedPlayers[i].list.Contains(player)) return triggers[i];
	}

	return triggers[0];
}
