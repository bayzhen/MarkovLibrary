// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MobileInputSettings.generated.h"

/**
 * 
 */
UCLASS(config = Input, defaultconfig)
class MARKOVLIBRARY_API UMobileInputSettings : public UObject
{
	GENERATED_UCLASS_BODY()
	UPROPERTY(config, EditAnywhere, Category = "Mobile", meta = (AllowedClasses = "/Script/Engine.TouchInterface"))
	FSoftObjectPath OverrideTouchInterface;
};