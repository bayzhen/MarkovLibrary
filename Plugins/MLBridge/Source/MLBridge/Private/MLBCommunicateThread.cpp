#include "MLBCommunicateThread.h"
#include "HAL/RunnableThread.h"
#include "SocketSubsystem.h"

#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

FMLBCommunicateThread* FMLBCommunicateThread::self = nullptr;

FMLBCommunicateThread::FMLBCommunicateThread(const FString& InIPAddress, int32 InPort)
    : IPAddress(InIPAddress), Port(InPort)
{
    bStopThread = false;
    Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

    bool bIsValid;
    Addr->SetIp(*IPAddress, bIsValid);
    Addr->SetPort(Port);

    FRunnableThread::Create(this, TEXT("MLBCommunicateThread"), 0, TPri_BelowNormal);
    self = this;
    Finished = false;
}

FMLBCommunicateThread::~FMLBCommunicateThread()
{
    Stop();
}

uint32 FMLBCommunicateThread::Run()
{
    TryReconnect();

    while (!bStopThread)
    {
        //MLBManager = AMLBManager::GetInstance();
        if (Socket != nullptr && Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
        {
            Socket->SetNonBlocking();
            TArray<uint8> Data;
            Data.SetNumUninitialized(ReceiveBufferSize);
            int32 BytesRead;
            FString ReceivedData;
            while (Socket->Recv(Data.GetData(), Data.Num(), BytesRead))
            {
                Data.SetNum(BytesRead);
                Data.Add(0);
                FString DataPart = UTF8_TO_TCHAR(reinterpret_cast<const ANSICHAR*>(Data.GetData()));
                if (DataPart.IsEmpty())
                    break; 
                else {
                    ReceivedData += DataPart;
                }
            }
            if (!ReceivedData.IsEmpty()) {
                ProcessIncomingData(ReceivedData);
            }
            while (!ObsQueue.IsEmpty()) {
                FString Obs;
                ObsQueue.Dequeue(Obs);
                this->SendData(Obs);
            }
        }
        FPlatformProcess::Sleep(0.01f);
    }
    Finished = true;
    UE_LOG(LogTemp, Log, TEXT("FMLBCommunicateThread Run Finished."));
    return 0;
}

void FMLBCommunicateThread::Stop()
{
    bStopThread = true;
    UE_LOG(LogTemp, Log, TEXT("FMLBCommunicateThread Stop."));
    while (true) {
        if (Finished)
            return;
        FPlatformProcess::Sleep(0.01f);
    }
}

void FMLBCommunicateThread::Exit()
{
    bStopThread = true;
    UE_LOG(LogTemp, Log, TEXT("FMLBCommunicateThread Exit."));
        while (true) {
        if (Finished)
            return;
        FPlatformProcess::Sleep(0.01f);
    }
}

void FMLBCommunicateThread::EnqueueCommand(const FString& Command)
{
    CommandQueue.Enqueue(Command);
}

bool FMLBCommunicateThread::DequeueCommand(FString& OutCommand)
{
    return CommandQueue.Dequeue(OutCommand);
}

void FMLBCommunicateThread::SendObs(const FString& Data)
{
    this->ObsQueue.Enqueue(Data);
}

FMLBCommunicateThread* FMLBCommunicateThread::GetThread()
{
    return self;
}

bool FMLBCommunicateThread::SendData(const FString& Data)
{
    if (Socket != nullptr && Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
    {
        int32 BytesSent;
        TArray<uint8> Payload;

        // 将FString转换为UTF-8格式的字符串
        FTCHARToUTF8 DataUTF8(*Data);

        // 将字符串附加到Payload中
        Payload.Append(reinterpret_cast<const uint8*>(DataUTF8.Get()), DataUTF8.Length());

        return Socket->Send(Payload.GetData(), Payload.Num(), BytesSent);
    }

    return false;
}


void FMLBCommunicateThread::TryReconnect()
{
    Socket = TSharedPtr<FSocket>(ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("MLBCommunicateThread"), false));

    if (!Socket->Connect(*Addr))
    {
        // 不打印了。
        // UE_LOG(LogTemp, Warning, TEXT("Failed to connect to %s:%d"), *IPAddress, Port);
        Socket.Reset();
        FPlatformProcess::Sleep(0.5f); // Wait for 5 seconds before trying to reconnect
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Connected to %s:%d"), *IPAddress, Port);
    }
}

bool FMLBCommunicateThread::ProcessIncomingData(FString& InData)
{
    if (!InData.IsEmpty())
    {
        EnqueueCommand(InData);
        return true;
    }

    return false;
}

