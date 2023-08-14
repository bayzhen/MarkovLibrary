// Fill out your copyright notice in the Description page of Project Settings.


#include "SortActor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

// Sets default values
ASortActor::ASortActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SquareBlockSideLength = 150.0;
	Height = 100.0;
	StaticCubeScale = 1.0;
	MainQueueLength = 10;
	MinimumHeight = 3;
	MaximumHeight = 20;
	TimeStepDuration = 2.0;
	bRandom =false;
	Seed = 10;
	MyRandomStream.Initialize(Seed);
	HISMComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HISMComponent"));
	this->SetRootComponent(Cast<USceneComponent>(HISMComponent));
	HISMComponent->OnComponentHit.AddUnique(
}

void ASortActor::OnConstruction(const FTransform& Transform)
{
	MyRandomStream.Initialize(Seed);
	HISMComponent->SetStaticMesh(StaticMesh);
	this->Reset();
}


// Called when the game starts or when spawned
void ASortActor::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ASortActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
/// <summary>
/// 重置各各网格体的位置。
/// </summary>
void ASortActor::Reset()
{
	HISMComponent->ClearInstances();
	GeneratedRandomArray.SetNum(MainQueueLength);
	MainArray.SetNum(MainQueueLength);
	TempArray.SetNum(1);
	for (int32 i = 0; i < GeneratedRandomArray.Num(); i++) {
		GeneratedRandomArray[i] = MyRandomStream.RandRange(MinimumHeight, MaximumHeight);
	}
	int32 count = 0;
	for (int32 i = 0; i < MainArray.Num(); i++) {
		MainArray[i] = count;
		count += GeneratedRandomArray[i];
		for (int32 j = 0; j < GeneratedRandomArray[i]; j++) {
			FTransform Transform;
			Transform.SetLocation(FVector(0.0, i * SquareBlockSideLength, j * Height));
			HISMComponent->AddInstance(Transform);
		}
	}
}

