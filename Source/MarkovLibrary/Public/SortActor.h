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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		float BaseDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		float Height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		float StaticCubeScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		int32 NumOfPillars;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		int32 MinimumHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		int32 MaximumHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		bool bRandom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		int32 Seed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		FRandomStream MyRandomStream;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		UStaticMesh* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		UHierarchicalInstancedStaticMeshComponent* HISMComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		UCurveFloat* CurveFloat;

	UFUNCTION(BlueprintCallable, Category = "SortActor")
		void Reset();
	UFUNCTION(BlueprintCallable, Category = "SortActor")
		void UpdatePillarTransform(int32 StartInstanceIndex, FTransform Transform);
	UFUNCTION(BlueprintCallable, Category = "SortActor")
		int32 GetPillarIndexByInstanceIndex(int32 ItemIndex);
	UFUNCTION(BlurprintCallable, Category = "SortActor")
		void TickMove(float Delta);
	UFUNCTION(BlurprintCallable, Category = "SortActor")
		void GetColumnTransform(int32 ColumnIndex);
	UFUNCTION(BlurprintCallable, Category = "SortActor")
		void SetSmoothMove(int32 ColumnIndex, FTransform EndTransform, float Duration);
	UFUNCTION(BlurprintCallable, Category = "SortActor")
		bool IsSorted();

	UFUNCTION(BlurprintCallable, Category = "SortActor")
		FTransform GetPillarTransformByPillarIndex(int32 PillarIndex);
	// Base是基座，基座上面放得柱子。
	UFUNCTION(BlurprintCallable, Category = "SortActor")
		FTransform GetBaseTransform(int32 BaseIndex);

	UFUNCTION(BlurprintCallable, Category = "SortActor")
		void MovePillarToBase(int32 PillarIndex, int32 BaseIndex);


private:
	// 一开始生成的随机数数组，这个数组不会动。数字代表着这一柱实例的数量。
	TArray<int32> PillarHeightArr;
	// 这个数组里存储的是根实例的序号。
	TArray<int32> PillarRootInstanceIndexArr;
	// 排序后的柱子的Index。
	TArray<int32> SortedPillarIndexArr;
	// 时间期间
	TArray<float> StepDurationArr;
	// 已消耗的时间
	TArray<float> StepTimeElapsedArr;
	// 开始位置
	TArray<FTransform> StepPillarStartTransformArr;
	// 目标位置
	TArray<FTransform> StepPillarEndTransformArr;
	// Alpha
	TArray<float> StepAlphaArr;

	bool bGaming;

};
