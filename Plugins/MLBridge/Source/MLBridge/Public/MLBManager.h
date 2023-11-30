#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Networking/Public/Networking.h"
#include "Sockets.h"
#include "IPAddress.h"
#include "SocketSubsystem.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Misc/DateTime.h"

#include "MLBSettings.h"
#include "MLBCommunicateThread.h"

#include "MLBManager.generated.h"


// The ML Bridge Manager connects, registers, and tunes the game for training, 
// sends information to the server, executes instructions, 
// and offers controlled acceleration to improve training efficiency while avoiding errors.
UCLASS()
class MLBRIDGE_API AMLBManager : public APawn
{
	GENERATED_BODY()

public:

	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds);

	void StepEnv(TArray<float>& actions);


public:
	// http专用
	void HttpSend();
	void HttpReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

public:
	// 控制
	UFUNCTION(BlueprintCallable)
	void PressKeys(TArray<float> InputValues);

	UFUNCTION(BlueprintCallable)
	void PressKey(FKey Key);

	void ReleaseKey(FKey Key);

	void MoveMouse(FVector2D Delta);

	APlayerController* GetPlayerController();

	TObjectPtr<UPlayerInput> GetPlayerInput();
public:
	// Json
	FString GetObsJsonString();
public:
	// 蓝图实现函数， 必须实现
	UFUNCTION(BlueprintImplementableEvent, Category = "MLBridge")
	TArray<float> GetObsArray();	
	
	UFUNCTION(BlueprintImplementableEvent, Category = "MLBridge")
	bool CanStartTraining();

	// 获取当前，在步骤前后做差，可以用来计算奖励
	UFUNCTION(BlueprintImplementableEvent, Category = "MLBridge")
	float GetCurrentValue();

	UFUNCTION(BlueprintImplementableEvent, Category = "MLBridge")
	bool IsTerminated();

	UFUNCTION(BlueprintImplementableEvent, Category = "MLBridge")
	bool IsTruncated();

	UFUNCTION(BlueprintImplementableEvent, Category = "MLBridge")
	bool ResetEnv();

public:
	// 蓝图调用函数， 可调可不调
	UFUNCTION(BlueprintCallable, Category = "MLBridge")
	void AddReward(float Reward);

	UFUNCTION(BlueprintCallable, Category = "MLBridge")
	void RegisterReturnObs();

	UFUNCTION(BlueprintCallable, Category = "MLBridge")
	void SocketReconnect();
public:
	// 通用设置
	UMLBSettings* Settings;

	FSocket* Socket;
public:
	// ML参数
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MLBridge")
	float ValueBeforeStep;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MLBridge")
	float RewardAccumulation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MLBridge")
	bool IsTraining;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MLBridge")
	bool NeedToReturnObs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MLBridge")
	float ValueDifferenceReward;

	FDateTime TimeRecord;

	int Count;

	FMLBCommunicateThread* MLBCommunicateThread;

	TSharedPtr<FJsonObject> ParseFStringIntoJson(FString& jsonString);

	TSharedPtr<FJsonObject> JsonObjectPtr;
};
