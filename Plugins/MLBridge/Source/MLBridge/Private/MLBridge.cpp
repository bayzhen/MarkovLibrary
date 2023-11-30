// Copyright Epic Games, Inc. All Rights Reserved.

#include "MLBridge.h"
#include "MLBManager.h"
#include "MLBSettings.h"
#include "ISettingsModule.h"
#define LOCTEXT_NAMESPACE "FMLBridgeModule"

/// <summary>
/// Ҫ��
///		ģ���������������AI�����Ƿ����ߡ�
///		һ���������ֱ�ӹر���Ϸ��
///		���ڽ����������Python�Ǳߡ�
///		ÿһ��UE��log���ᱻ�����������Բ�����⡣
/// </summary>
void FMLBridgeModule::StartupModule()
{
	Settings = GetMutableDefault<UMLBSettings>();
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "MLBridge",
			LOCTEXT("RuntimeSettingsName", "MLBridge"), FText::FromString(TEXT("The Machine Learning Bridge streamlines communication and execution of instructions between the AI training machine and clients.")), 
			GetMutableDefault<UMLBSettings>());
	}
	/*MLBCommunicateThread = TSharedPtr<FMLBCommunicateThread>(new FMLBCommunicateThread(Settings->SocketIP, Settings->SocketPort));*/
}

void FMLBridgeModule::ShutdownModule()
{
	/*MLBCommunicateThread->Stop();*/
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMLBridgeModule, MLBridge)