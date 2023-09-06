
// Fill out your copyright notice in the Description page of Project Settings.


#include "MLPlayerController.h"

void AMLPlayerController::Travel(FString URL, ETravelType TravelType, bool bSeamless) {
	ClientTravel(URL, TravelType, bSeamless);
}