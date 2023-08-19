// Fill out your copyright notice in the Description page of Project Settings.


#include "SortActor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

// Sets default values
ASortActor::ASortActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InstanceDistance = 100.0;
	MinimumHeight = 3;
	MaximumHeight = 20;
	bRandom = false;
	Seed = 10;
	bGaming = true;
	HISMComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HISMComponent"));
	this->SetRootComponent(Cast<USceneComponent>(HISMComponent));
	// SortTable = TSharedPtr<FSortTable>(new FSortTable(
}

// Called when the game starts or when spawned
void ASortActor::BeginPlay()
{
	Super::BeginPlay();
	if (!bRandom) {
		MyRandomStream.Initialize(Seed);
	}
	else {
		MyRandomStream.Initialize(FMath::Rand());
	}
	HISMComponent->SetStaticMesh(StaticMesh);
	//HISMComponent->OnComponentHit.Add
	Reset();
}


// Called every frame
void ASortActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// 重置各各网格体的位置。
void ASortActor::Reset()
{

}

void ASortActor::GameStep(int32 PillarIndex)
{
}

int32 ASortActor::GetPillarIndexByInstanceIndex(int32 InstanceIndex)
{
	return 0;
}
