// Fill out your copyright notice in the Description page of Project Settings.


#include "RLEnvCommunicator.h"
#include "Sockets.h"
#include "Interfaces/IPv4/IPv4Address.h"

URLEnvCommunicator* URLEnvCommunicator::self = nullptr;

URLEnvCommunicator* URLEnvCommunicator::Get()
{
	if (!self)
	{
		self = NewObject<URLEnvCommunicator>();
	}
	return self;
}


URLEnvCommunicator::URLEnvCommunicator()
{	
	Connected = false;
	Socket = nullptr;
    self = this;
}

bool URLEnvCommunicator::Connect()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("UE4_TCP_Client"), false);
	FIPv4Address IPAddress;
	FIPv4Address::Parse(TEXT("127.0.0.1"), IPAddress);
	TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Addr->SetIp(IPAddress.Value);
	Addr->SetPort(12345);

	Connected = Socket->Connect(*Addr);
	if (Connected) {
		UE_LOG(LogTemp, Warning, TEXT("Markov: Socket connected."));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Markov: Socket not connected."));
	}
	self = this;
	return Connected;
}

bool URLEnvCommunicator::Disconnect()
{
	Connected = false;
	return Socket->Close();
}


URLEnvCommunicator::~URLEnvCommunicator()
{
	Connected = false;
	// ¹Ø±ÕÌ×½Ó×Ö
	if(Socket)
	Socket->Close();
}

void URLEnvCommunicator::SendFString(const FString& Input)
{
	if (Connected) {
		int32 BytesSent;
		Socket->Send((uint8*)TCHAR_TO_UTF8(*Input), Input.Len(), BytesSent);
	}
}

FString URLEnvCommunicator::GetFString()
{
	if (Connected)
	{
		TArray<uint8> ReceivedData;
		uint32 Size;

		while (Socket->HasPendingData(Size))
		{
			ReceivedData.Init(0, Size);
			int32 Read = 0;
			Socket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);
		}

		if (ReceivedData.Num() > 0)
		{
			FString ReceivedString = FString(ReceivedData.Num(), (TCHAR*)(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceivedData.GetData()))));
			UE_LOG(LogTemp, Log, TEXT("Received String: %s"), *ReceivedString);
			return ReceivedString;
		}
		else {
			return "";
		}
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Not connected."));
		return "";
	}
}

bool URLEnvCommunicator::GetConnectState()
{
	if (!Socket)
		return false;

	ESocketConnectionState ConnectionState = Socket->GetConnectionState();

	switch (ConnectionState)
	{
	case ESocketConnectionState::SCS_Connected:
		return true;

	case ESocketConnectionState::SCS_NotConnected:
		break;

	case ESocketConnectionState::SCS_ConnectionError:
		break;

	default:
		break;
	}
	return false;
}
