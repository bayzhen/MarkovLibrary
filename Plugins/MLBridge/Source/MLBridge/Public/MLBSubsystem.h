#pragma once

#include "Subsystems/Subsystem.h"
#include "MLBSettings.h"
#include "MLBSubsystem.generated.h"

UCLASS()
class MLBRIDGE_API UMLBSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable)
    static UMLBSubsystem* Get();

    UFUNCTION(BlueprintCallable)
    UWorld* GetRunningWorld();

    UFUNCTION(BlueprintCallable)
    UPlayerInput* GetPlayerInput();

    UFUNCTION(BlueprintCallable)
    void PressKey(FName KeyName, float Duration); 

    UFUNCTION(BlueprintCallable)
    void ReleaseKey(FName KeyName);
    // Add your custom functions here

    static UMLBSubsystem* self;
};