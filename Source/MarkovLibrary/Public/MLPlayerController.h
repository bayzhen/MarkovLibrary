// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MLPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MARKOVLIBRARY_API AMLPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void Travel(FString URL, ETravelType TravelType, bool bSeamless);
};
