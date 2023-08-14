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
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Reset();

	UPROPERTY(EditAnywhere)
		float SquareBlockSideLength;
	UPROPERTY(EditAnywhere)
		float Height;
	UPROPERTY(EditAnywhere)
		float StaticCubeScale;
	UPROPERTY(EditAnywhere)
		int32 MainQueueLength;
	UPROPERTY(EditAnywhere)
		int32 MinimumHeight;
	UPROPERTY(EditAnywhere)
		int32 MaximumHeight;
	UPROPERTY(EditAnywhere)
		float TimeStepDuration;
	/// <summary>
	/// 一开始生成的随机数数组，这个数组不会动。数字代表着这一柱实例的数量。
	/// </summary>
	UPROPERTY(EditAnywhere)
		TArray<int32> GeneratedRandomArray;
	/// <summary>
	/// 这个数组里存储的是根实例的序号。
	/// </summary>
	UPROPERTY(EditAnywhere)
		TArray<int32> MainArray;
	/// <summary>
	/// 这个数组里用于存放拿出来的柱子。
	/// </summary>
	UPROPERTY(EditAnywhere)
		TArray<int32> TempArray;
	UPROPERTY(EditAnywhere)
		bool bRandom;
	UPROPERTY(EditAnywhere)
		int32 Seed;
	UPROPERTY(EditAnywhere)
		FRandomStream MyRandomStream;
	UPROPERTY(EditAnywhere)
		UStaticMesh* StaticMesh;
	UPROPERTY(EditAnywhere)
		UHierarchicalInstancedStaticMeshComponent* HISMComponent;

	UFUNCTION(BlueprintCallable)
		void Move(int32 StartInstanceIndex, FTransform& Transform);
};
