// Fill out your copyright notice in the Description page of Project Settings.


#include "Prim.h"
#include "Math/UnrealMathUtility.h"

Prim::Prim()
{
}

Prim::~Prim()
{
}

Prim::Prim(int32 Height, int32 Weight, int32 Seed)
{
	if (!(Height && Weight)) {
		return;
	}
	H = Height;
	W = Weight;
	FMath::RandInit(Seed);
	for (int32 h = 0; h < 2*Height+1; h++)
	{
		TArray<bool> T;
		for (int32 w = 0; w <2* Weight+1; w++)
		{
			T.Add(false);
		}
		Maze.Add(T);
	}
	Prim::Generate();
}

void Prim::Generate() {
	Maze[1][1] = true;
	ExploredPoints.Add(FIntVector2D(0, 0));
	while (!ExploredPoints.IsEmpty()) {
		int32 Index = FMath::RandRange(0, ExploredPoints.Num() - 1);
		FIntVector2D& Point = ExploredPoints[Index];
		int32 X = Point.X;
		int32 Y = Point.Y;
		TArray<FIntVector2D> AvailablePoints;
		if (X > 0 && !Maze[2 * (X - 1) + 1][2 * Y + 1])
			AvailablePoints.Add(FIntVector2D(X - 1, Y));
		if (X < H - 1 && !Maze[2 * (X + 1) + 1][2 * Y + 1])
			AvailablePoints.Add(FIntVector2D(X + 1, Y));
		if (Y > 0 && !Maze[2 * X + 1][2 * (Y - 1) + 1])
			AvailablePoints.Add(FIntVector2D(X, Y - 1));
		if (Y < W - 1 && !Maze[2 * X + 1][2 * (Y + 1) + 1])
			AvailablePoints.Add(FIntVector2D(X, Y + 1));
		if (!AvailablePoints.Num())
		{
			ExploredPoints.RemoveAt(Index);
			continue;
		}
		else {
			FIntVector2D& BreakPoint = AvailablePoints[FMath::RandRange(0, AvailablePoints.Num() - 1)];
			X = (X + BreakPoint.X) +1;
			Y = (Y + BreakPoint.Y) +1;
			Maze[X][Y] = true;
			Maze[2*BreakPoint.X+1][2*BreakPoint.Y+1] = true;
			ExploredPoints.Add(BreakPoint);
		}
	}
}