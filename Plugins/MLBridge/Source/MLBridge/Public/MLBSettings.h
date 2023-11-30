#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/PlayerInput.h"
#include "MLBSettings.generated.h"

UENUM(BlueprintType)
enum class EConnectionMethod : uint8
{
	Socket UMETA(DisplayName = "Socket"),
	Http UMETA(DisplayName = "Http"),
	Https UMETA(DisplayName = "Https")
};

UCLASS(config = MLBridge, defaultconfig)
class MLBRIDGE_API UMLBSettings : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "MLBridge")
	EConnectionMethod ConnectionMethod;

	UPROPERTY(config, EditAnywhere, Category = "MLBridge")
	FString SocketIP;

	UPROPERTY(config, EditAnywhere, Category = "MLBridge")
	int SocketPort;

	UPROPERTY(config, EditAnywhere, Category = "MLBridge")
	FString HttpURL;

	UPROPERTY(config, EditAnywhere, Category = "MLBridge")
	FString HttpsURL;

	UPROPERTY(config, EditAnywhere, Category = "MLBridge")
	float MLBDeltaTime;

	// 键盘鼠标所有输入
	UPROPERTY(config, EditAnywhere, Category = "MLBridge", Meta = (ToolTip = "The time duration of an action."))
	TArray<FName> InputKeys;

private:
	static UMLBSettings* self;
};
