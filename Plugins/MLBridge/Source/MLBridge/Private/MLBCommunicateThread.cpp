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

	Thread =  FRunnableThread::Create(this, TEXT("MLBCommunicateThread"), 0, TPri_BelowNormal);
}

FMLBCommunicateThread::~FMLBCommunicateThread()
{
	delete Thread;
}

uint32 FMLBCommunicateThread::Run()
{
	TryReconnect();
	while (!bStopThread)
	{
		if (Socket != nullptr && Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
		{
			Socket->SetNonBlocking();
			TArray<uint8> Data;
			Data.SetNumUninitialized(ReceiveBufferSize);
			int32 BytesRead =0 ;
			FString ReceivedData;
			while (Socket->Recv(Data.GetData(), Data.Num(), BytesRead))
			{
				Data.SetNum(BytesRead);
				Data.Add(0);
				const char* data = reinterpret_cast<char*>(Data.GetData());
				FString DataPart = StringCast<UTF8CHAR>(data).Get();
				if (DataPart.IsEmpty())
					break;
				else {
					ReceivedData += *DataPart;
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
	UE_LOG(LogTemp, Log, TEXT("FMLBCommunicateThread Run Finished."));
	return 0;
}

void FMLBCommunicateThread::Stop()
{
	bStopThread = true;
	UE_LOG(LogTemp, Log, TEXT("FMLBCommunicateThread Stop."));
}

void FMLBCommunicateThread::Exit()
{
	bStopThread = true;
	UE_LOG(LogTemp, Log, TEXT("FMLBCommunicateThread Exit."));
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

		// ��FStringת��ΪUTF-8��ʽ���ַ���
		FTCHARToUTF8 DataUTF8(*Data);

		// ���ַ������ӵ�Payload��
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
		// ����ӡ�ˡ�
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

