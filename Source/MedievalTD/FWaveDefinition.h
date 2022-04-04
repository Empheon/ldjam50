#pragma once

#include "CoreMinimal.h"
#include "MonsterAI.h"
#include "FWaveDefinition.generated.h"

UENUM(BlueprintType)
enum ESpawnSide
{
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
};



USTRUCT(BlueprintType)
struct FMobInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMonsterAI> MobClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Count = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level;

	inline bool operator==(FMobInfo a) {
		return a.MobClass == MobClass && a.Count == Count && a.Level == Level;
	}
};

USTRUCT(BlueprintType)
struct FWaveDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMobInfo> MobInfos;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TotalSpawnTime = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<ESpawnSide>> EnabledSpawnSides;
};
