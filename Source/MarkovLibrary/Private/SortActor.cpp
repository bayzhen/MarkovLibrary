// Fill out your copyright notice in the Description page of Project Settings.


#include "SortActor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

// Sets default values
ASortActor::ASortActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NumOfBases = 10;
	InstanceDistance = 100.0;
	MinimumHeight = 3;
	MaximumHeight = 20;
	bRandom = true;
	Seed = 10;
	bGaming = true;
	HISMComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HISMComponent"));
	this->SetRootComponent(Cast<USceneComponent>(HISMComponent));
	SortTable = MakeShared<FSortTable>();
}

TArray<FTransform> ASortActor::GenerateBasesTransforms_Implementation()
{
	TArray<FTransform> TransformArr;
	TransformArr.Init(FTransform(), NumOfBases);
	FVector HISMComponentLocation = HISMComponent->K2_GetComponentLocation();
	for (int32 i = 0; i < NumOfBases; i++) {
		FVector BaseLocation = HISMComponentLocation + FVector(0.0f, i * 150.0f, 0.0);
		TransformArr[i].SetLocation(BaseLocation);
	}
	return TransformArr;
}

// Called when the game starts or when spawned
void ASortActor::BeginPlay()
{
	Super::BeginPlay();
	Reset();
}

void ASortActor::OnConstruction(const FTransform& Transform)
{
	Reset();
}


// Called every frame
void ASortActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SortTable->Tick(DeltaTime);
}

// 重置各各网格体的位置。
void ASortActor::Reset()
{
	HISMComponent->SetStaticMesh(StaticMesh);
	SortTable->GenerateBases(NumOfBases, GenerateBasesTransforms(), MinimumHeight, MaximumHeight, HISMComponent, InstanceDistance, XFloatCurve, YFloatCurve, ZFloatCurve,bRandom, Seed);
}

void ASortActor::GameStep(int32 PillarIndex)
{

}

FBase ASortActor::GetBaseByPillarInstanceIndex(int32 InstanceIndex)
{
	for (int32 i = 0; i < SortTable->BaseArr.Num();i++) {
		auto& Base = SortTable->BaseArr[i];
		if (Base.Pillar->InstanceIndexArr.Contains(InstanceIndex)) {
			return Base;
		};
	}
	return SortTable->BaseArr[0];
}
