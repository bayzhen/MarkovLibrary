// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Prim.h"
#include "Engine/StaticMesh.h"
#include "MyActor.generated.h"

UCLASS()
class MARKOVLIBRARY_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void Reset();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* MyStaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Seed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Gap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UHierarchicalInstancedStaticMeshComponent* HISMComponent;

	UFUNCTION(BlueprintPure)
		bool IsWall(int32 x, int32 y);
private:
	TArray<TArray<bool>> Maze;
};
