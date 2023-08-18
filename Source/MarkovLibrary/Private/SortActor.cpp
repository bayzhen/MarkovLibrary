// Fill out your copyright notice in the Description page of Project Settings.


#include "SortActor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

// Sets default values
ASortActor::ASortActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseDistance = 150.0;
	Height = 100.0;
	StaticCubeScale = 1.0;
	NumOfPillars = 10;
	MinimumHeight = 3;
	MaximumHeight = 20;
	bRandom = false;
	Seed = 10;
	ChairTransform = FTransform(FVector(-100.0f, -100.0f, 0.0f));
	SelectedPillarIndex = -1;
	AvailableBase = -1;
	HISMComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HISMComponent"));
	this->SetRootComponent(Cast<USceneComponent>(HISMComponent));
}

void ASortActor::OnConstruction(const FTransform& Transform)
{
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
	TickMove(DeltaTime);
}

// 重置各各网格体的位置。
void ASortActor::Reset()
{
	HISMComponent->ClearInstances();
	PillarHeightArr.SetNum(NumOfPillars);
	PillarRootInstanceIndexArr.SetNum(NumOfPillars);
	for (int32 i = 0; i < NumOfPillars; i++) {
		PillarHeightArr[i] = MyRandomStream.RandRange(MinimumHeight, MaximumHeight);
	}
	int32 count = 0;
	for (int32 i = 0; i < NumOfPillars; i++) {
		PillarRootInstanceIndexArr[i] = count;
		count += PillarHeightArr[i];
		for (int32 j = 0; j < PillarHeightArr[i]; j++) {
			FTransform Transform = GetBaseTransform(i);
			FVector Location = Transform.GetLocation();
			Location += FVector(0, 0, j * Height);
			Transform.SetLocation(Location);
			int32 Index = HISMComponent->AddInstance(Transform);
			UE_LOG(LogTemp, Log, TEXT("Index: %d"), Index);
		}
	}
	StepDurationArr.Init(0.0f, NumOfPillars);
	StepTimeElapsedArr.Init(0.0f, NumOfPillars);
	StepPillarStartTransformArr.Init(FTransform(), NumOfPillars);
	StepPillarEndTransformArr.Init(FTransform(), NumOfPillars);
	StepAlphaArr.Init(1.0f, NumOfPillars);
	for (int32 i = 0; i < NumOfPillars; i++) {
		FTransform Transform;
		HISMComponent->GetInstanceTransform(PillarRootInstanceIndexArr[i], Transform, true);
		StepPillarStartTransformArr[i] = Transform;
		StepPillarEndTransformArr[i] = Transform;
	}
}

void ASortActor::UpdatePillarTransform(int32 PillarIndex, FTransform Transform)
{
	FTransformArrayA2 TransformArr;
	TransformArr.SetNum(PillarHeightArr[PillarIndex]);
	for (int32 i = 0; i < PillarHeightArr[PillarIndex]; i++) {
		FTransform TempTransform = Transform;
		TempTransform.SetLocation(Transform.GetLocation() + FVector(0.0, 0.0, i * Height));
		TransformArr[i] = TempTransform;
	}
	HISMComponent->BatchUpdateInstancesTransforms(PillarRootInstanceIndexArr[PillarIndex], TransformArr, true);
}



void ASortActor::TickMove(float Delta) {
	check(StepDurationArr.Num() == StepTimeElapsedArr.Num());
	for (int32 i = 0; i < NumOfPillars; i++) {
		float Alpha = StepTimeElapsedArr[i] / StepDurationArr[i];
		if (Alpha <= 1) {
			StepTimeElapsedArr[i] += Delta;
			StepAlphaArr[i] = FMath::Clamp(StepTimeElapsedArr[i] / StepDurationArr[i], 0.0f, 1.0f);
			UpdatePillarTransform(i, FTransform(FMath::Lerp(StepPillarStartTransformArr[i].GetLocation(), StepPillarEndTransformArr[i].GetLocation(), StepAlphaArr[i])));
		}
	}
}

void ASortActor::GetColumnTransform(int32 ColumnIndex)
{
}

void ASortActor::SetSmoothMove(int32 ColumnIndex, FTransform EndTransform, float Duration)
{
	StepDurationArr[ColumnIndex] = Duration;
	StepTimeElapsedArr[ColumnIndex] = 0.0f;
}

bool ASortActor::IsSorted()
{
	int32 count = -1;
	for (auto& Index : SortedPillarIndexArr) {
		if (PillarHeightArr[Index] < count)
			return false;
		else
			count = PillarHeightArr[Index];
	}
	return true;
}

FTransform ASortActor::GetPillarTransformByPillarIndex(int32 PillarIndex)
{
	check(PillarIndex < NumOfPillars);
	int32 InstanceIndex = PillarRootInstanceIndexArr[PillarIndex];
	FTransform PillarTransform;
	HISMComponent->GetInstanceTransform(InstanceIndex, PillarTransform, true);
	return PillarTransform;
}

FTransform ASortActor::GetBaseTransform(int32 BaseIndex)
{
	FTransform Transform;
	Transform.SetLocation(FVector(0.0, BaseIndex * BaseDistance, 0.0));
	return Transform;
}

void ASortActor::MovePillarToBase(int32 PillarIndex, int32 BaseIndex)
{
	FTransform PillarTransform = GetPillarTransformByPillarIndex(PillarIndex);
	FTransform BaseTransform;
	if (BaseIndex >= 0)
		BaseTransform = GetBaseTransform(BaseIndex);
	else
		FTransform BaseTransform = ChairTransform;
	StepPillarStartTransformArr[PillarIndex] = PillarTransform;
	StepPillarStartTransformArr[PillarIndex] = BaseTransform;
	StepTimeElapsedArr[PillarIndex] = 0;
}

void ASortActor::GameStep(int32 PillarIndex)
{
	MovePillarToBase(PillarIndex, AvailableBase);
}

int32 ASortActor::GetPillarIndexByInstanceIndex(int32 InstanceIndex)
{
	int32 Index = -1;
	for (int32 i = 0; i < NumOfPillars; i++) {
		if (PillarRootInstanceIndexArr[i] > InstanceIndex) {
			Index = i - 1;
			break;
		}
	}
	if (Index == -1)Index = PillarRootInstanceIndexArr.Num() - 1;
	return Index;
}
