// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "SortActor.generated.h"


USTRUCT(BlueprintType)
struct MARKOVLIBRARY_API FPillar
{
	GENERATED_BODY()
public:
	TSharedPtr<FBase> Base;
	int32 Height;
	UHierarchicalInstancedStaticMeshComponent* HISMComponent;
	float InstanceDistance;
	TArray<int32> InstanceIndexArr;
	FTransform Transform;
	FTransform StartTransform;
	FTransform EndTransform;
	float TimeExplased;
	FFloatCurve XFloatCurve;
	FFloatCurve YFloatCurve;
	FFloatCurve ZFloatCurve;
	float Duration;
	FPillar() {}
	FPillar(
		TSharedPtr<FBase> Base,
		int32 Height,
		UHierarchicalInstancedStaticMeshComponent* HISMComponent,
		float InstanceDistance,
		FTransform BaseTransform,
		FFloatCurve& XFloatCurve,
		FFloatCurve& YFloatCurve,
		FFloatCurve& ZFloatCurve,
		float Duration = 0.5
	) :
		Base(Base),
		Height(Height),
		HISMComponent(HISMComponent),
		InstanceDistance(InstanceDistance),
		Transform(BaseTransform),
		StartTransform(BaseTransform),
		EndTransform(BaseTransform),
		XFloatCurve(XFloatCurve),
		YFloatCurve(YFloatCurve),
		ZFloatCurve(ZFloatCurve),
		Duration(Duration)
	{
		InstanceIndexArr.Init(0, Height);
		for (int32 i = 0; i < Height; i++) {
			FVector Location = Transform.GetLocation();
			Location.Z += i * InstanceDistance;
			Transform.SetLocation(Location);
			int32 Index = HISMComponent->AddInstance(Transform, true);
			InstanceIndexArr[i] = Index;
		}
		TimeExplased = 0.0f;
	}
	void Tick(float Delta)
	{
		if (TimeExplased < Duration) {
			TimeExplased += Delta;
		}
		else {
			return;
		}
		float Alpha = FMath::Clamp(TimeExplased / Duration, 0.0f, 1.0f);
		float XAlpha = XFloatCurve.Evaluate(Alpha);
		float YAlpha = YFloatCurve.Evaluate(Alpha);
		float ZAlpha = ZFloatCurve.Evaluate(Alpha);
		FVector StartLocation = StartTransform.GetLocation();
		float StartX = StartLocation.X;
		float StartY = StartLocation.Y;
		float StartZ = StartLocation.Z;
		FVector EndLocation = EndTransform.GetLocation();
		float EndX = EndLocation.X;
		float EndY = EndLocation.Y;
		float EndZ = EndLocation.Z;
		float X = FMath::Lerp(StartX, EndX, Alpha);
		float Y = FMath::Lerp(StartY, EndY, Alpha);
		float Z = FMath::Lerp(StartZ, EndZ, Alpha);
		Transform.SetLocation(FVector(X, Y, Z));
		for (int32 i = 0; i < InstanceIndexArr.Num(); i++) {
			HISMComponent->BatchUpdateInstancesTransform(InstanceIndexArr[0], InstanceIndexArr.Num(), Transform, true);
		}
	}
};

USTRUCT(BlueprintType)
struct MARKOVLIBRARY_API FBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		FTransform Transform;
	TSharedPtr<FPillar> Pillar;
	FBase() {}
	FBase(FTransform Transform) :Transform(Transform) {}
	void GeneratePillar(FRandomStream& RandomStream,
		int32 MinimumHeight,
		int32 MaximumHeight,
		UHierarchicalInstancedStaticMeshComponent* HISMComponent,
		float InstanceDistance,
		FFloatCurve& XFloatCurve,
		FFloatCurve& YFloatCurve,
		FFloatCurve& ZFloatCurve,
		float Duration = 0.5)
	{
		int32 Height = RandomStream.RandRange(MinimumHeight, MaximumHeight);
		TSharedPtr<FPillar> ItsPillar(new FPillar(TSharedPtr<FBase>(this), Height, HISMComponent, InstanceDistance, Transform, XFloatCurve, YFloatCurve, ZFloatCurve, Duration));
		Pillar = ItsPillar;
	}
	void Tick(float Delta) {
		Pillar->Tick(Delta);
	}
};

USTRUCT(BlueprintType)
struct MARKOVLIBRARY_API FSortTable
{
	GENERATED_BODY()
public:
	TArray<FBase> BaseArr;
	FRandomStream RandomStream;
	FSortTable() {}
	void GenerateBases(int32 Num,
		TArray<FTransform> Transforms,
		int32 MinimumHeight,
		int32 MaximumHeight,
		UHierarchicalInstancedStaticMeshComponent* HISMComponent,
		float InstanceHeight,
		FFloatCurve XFloatCurve,
		FFloatCurve YFloatCurve,
		FFloatCurve ZFloatCurve,
		bool bRandom = true,
		int32 Seed = 0)
	{
		if (!bRandom)
			RandomStream.Initialize(Seed);
		BaseArr.SetNum(Num);
		for (int32 i = 0; i < Num; i++) {
			BaseArr[i] = FBase(Transforms[i]);
			BaseArr[i].GeneratePillar(RandomStream,
				MinimumHeight,
				MaximumHeight,
				HISMComponent,
				InstanceHeight,
				XFloatCurve,
				YFloatCurve,
				ZFloatCurve);
		}
	}

	void Switch(int32 AIndex, int32 BIndex) {
		// Base的指针不会变，所以可以用引用。
		// Pillar的指针会变，所以不用引用。
		auto& ABase = BaseArr[AIndex];
		auto& BBase = BaseArr[BIndex];
		auto& ABaseTransform = ABase.Transform;
		auto& BBaseTransform = BBase.Transform;
		auto APillar = ABase.Pillar;
		auto BPillar = BBase.Pillar;
		auto APillarTransform = ABase.Pillar->Transform;
		auto BPillarTransform = BBase.Pillar->Transform;
		APillar->StartTransform = APillarTransform;
		APillar->EndTransform = BBaseTransform;
		APillar->TimeExplased = 0.0f;
		BPillar->StartTransform = BPillarTransform;
		BPillar->EndTransform = ABaseTransform;
		BPillar->TimeExplased = 0.0f;
		ABase.Pillar = BPillar;
		BBase.Pillar = APillar;
	}

	void Tick(float Delta) {
		for (auto& Base : BaseArr) {
			Base.Tick(Delta);
		}
	}
};

UCLASS()
class MARKOVLIBRARY_API ASortActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASortActor();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		int32 NumOfBases;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		float InstanceDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		int32 MinimumHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		int32 MaximumHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		bool bRandom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		int32 Seed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		FRandomStream MyRandomStream;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		UStaticMesh* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		UHierarchicalInstancedStaticMeshComponent* HISMComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		FFloatCurve XFloatCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		FFloatCurve YFloatCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SortActor")
		FFloatCurve ZFloatCurve;

	UFUNCTION(BlueprintCallable, Category = "SortActor")
		void Reset();
	UFUNCTION(BlueprintCallable, Category = "SortActor")
		int32 GetPillarIndexByInstanceIndex(int32 ItemIndex);
	UFUNCTION(BlueprintCallable, Category = "SortActor")
		void GameStep(int32 PillarIndex);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SortActor")
		TArray<FTransform> GenerateBasesTransforms();
private:
	TSharedPtr<FSortTable> SortTable;
	bool bGaming;
};
