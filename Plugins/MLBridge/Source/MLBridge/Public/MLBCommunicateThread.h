#pragma once

#include "CoreMinimal.h"

#include "HAL/Runnable.h"
#include "Containers/Queue.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "MLBSubsystem.h"

class MLBRIDGE_API FMLBCommunicateThread : public FRunnable
{
public:
    FMLBCommunicateThread(const FString& IPAddress, int32 Port);
    virtual ~FMLBCommunicateThread();

    virtual uint32 Run() override;
    virtual void Stop() override;
    virtual void Exit() override;

    void EnqueueCommand(const FString& Command);
    bool DequeueCommand(FString& OutCommand);
    void SendObs(const FString& Data);
    static FMLBCommunicateThread* GetThread();

public:
    FThreadSafeBool bStopThread;

    FString IPAddress;
    int32 Port;

    TQueue<FString> CommandQueue;
    TQueue<FString> ObsQueue;

    TSharedPtr<FSocket> Socket;
    TSharedPtr<FInternetAddr> Addr;

    void TryReconnect();
    bool ProcessIncomingData(FString& InData);

    bool SendData(const FString& Data);
    bool Finished;
    const int32 ReceiveBufferSize = 1024;
    static FMLBCommunicateThread* self;
};
