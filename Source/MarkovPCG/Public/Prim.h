// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

struct FIntVector2D
{
    int32 X;
    int32 Y;
    FIntVector2D() : X(0), Y(0) {}
    FIntVector2D(int32 InX, int32 InY) : X(InX), Y(InY) {}
};


class MARKOVPCG_API Prim
{
public:
	Prim();
	~Prim();
	Prim(int32 Width, int32 Height, int32 Seed=1);
	void Generate();
public:
	TArray<TArray<bool>> Maze;
	TArray<FIntVector2D> ExploredPoints;
	int32 H, W;
};
