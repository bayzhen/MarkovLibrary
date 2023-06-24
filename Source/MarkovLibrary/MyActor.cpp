// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* NewRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	this->SetRootComponent(NewRootComponent);
	// Create component
	InstancedMeshComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HierarchicalInstancedStaticMeshComponent"));

	// Attach the component to the root component
	 InstancedMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyActor::GenerateMaze() {
	if (!(MyStaticMesh&&Width&&Height)) {
		UE_LOG(LogTemp, Warning, TEXT("Static Mesh not set up."));
		return;
	}
	Prim MyPrim(Width, Height, Seed);
	Maze = MyPrim.Maze;

	// Set the static mesh to use for instances
	InstancedMeshComponent->SetStaticMesh(MyStaticMesh);
	InstancedMeshComponent->ClearInstances();

	// Next, add instances to the component
	for (int32 i = 0; i < Maze.Num(); i++)
	{
		for (int32 j = 0; j < Maze[0].Num(); j++)
		{
			if (Maze[i][j])
				continue;
			// Create a transform for the instance
			FTransform InstanceTransform = FTransform::Identity;
			InstanceTransform.SetLocation(FVector(i * Gap, j * Gap, 0.f)+this->GetActorLocation()); // Set the location of the instance
			// Add the instance to the component
			InstancedMeshComponent->AddInstance(InstanceTransform);
		}
	}
}

bool AMyActor::IsWall(int32 x, int32 y)
{
	// Get a reference to the two-dimensional boolean array
	TArray<TArray<bool>>& WallArray = this->Maze;
	x = 2 * x + 1;
	y = 2 * y + 1;
	// Check if the specified position is within the bounds of the array
	if (x >= 0 && x < WallArray.Num() && y >= 0 && y < WallArray[x].Num())
	{
		// Return the value at the specified position in the array
		return WallArray[x][y];
	}

	// If the specified position is out of bounds, return false
	return false;
}