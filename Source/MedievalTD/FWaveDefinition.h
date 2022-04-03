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
struct FWaveDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TSubclassOf<AMonsterAI>, int> MobCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TotalSpawnTime = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<ESpawnSide>> EnabledSpawnSides;
};

