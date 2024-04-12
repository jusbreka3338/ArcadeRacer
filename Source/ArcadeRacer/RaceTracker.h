// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "RaceTracker.generated.h"

USTRUCT()
struct FActorEncapsulate
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY()
	TArray<AActor*> list;
};

UCLASS()
class ARCADERACER_API ARaceTracker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARaceTracker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<FActorEncapsulate> players;

	UPROPERTY()
	TArray<AActor*> playerActors;
	UPROPERTY()
	TArray<int> playerLaps;

	UPROPERTY()
	TArray<AActor*> triggers;

	void HandleTracking();
	UPROPERTY()
	TArray<AActor*> sortedPlayers;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SpawnTriggers(USplineComponent* inSpline);

	void PlayerEnterTrigger(AActor* inPlayer, int triggerIndex);

	UPROPERTY(EditAnywhere)
	UBlueprint* triggerBlueprint;

	UFUNCTION(BlueprintCallable)
	int RequestPosition(AActor* player);
	UFUNCTION(BlueprintCallable)
	AActor* RequestLastTrigger(AActor* player);
};
