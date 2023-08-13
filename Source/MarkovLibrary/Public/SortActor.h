// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SortActor.generated.h"

UCLASS()
class MARKOVLIBRARY_API ASortActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASortActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	int32 squareBlockSideLength;
	int32 staticCubeGridSideLength;
	int32 mainQueueLength;
	int32 minimumHeight;
	int32 maximumHeight;
	int32 timeStepDuration;
	TArray<int32> MainArray;
	TArray<int32> TempArray;
	bool bRandom;
	int32 Seed;
	FRandomStream MyRandomStream;

};
