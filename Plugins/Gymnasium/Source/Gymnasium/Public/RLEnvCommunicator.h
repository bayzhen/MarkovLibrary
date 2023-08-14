// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SocketSubsystem.h"
#include "RLEnvCommunicator.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GYMNASIUM_API URLEnvCommunicator : public UObject
{
    GENERATED_BODY()

public:
    static URLEnvCommunicator* Get();


    FSocket* Socket;

    URLEnvCommunicator();
    ~URLEnvCommunicator();
       
    UFUNCTION(BlueprintCallable, Category = "RLEnvCommunicator")
        bool Connect();

    UFUNCTION(BlueprintCallable, Category = "RLEnvCommunicator")
        bool Disconnect();

    UFUNCTION(BlueprintCallable, Category = "RLEnvCommunicator")
        void SendFString(const FString& Input);

    UFUNCTION(BlueprintCallable, Category = "RLEnvCommunicator")
        FString GetFString();

    UFUNCTION(BlueprintCallable, Category = "RLEnvCommunicator")
        bool GetConnectState();

    UPROPERTY(BlueprintReadOnly)
        bool Connected;



private:
    static URLEnvCommunicator* self;
};
