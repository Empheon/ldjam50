// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FWaveDefinition.h"
#include "GameFramework/GameModeBase.h"
#include "MedievalTDGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MEDIEVALTD_API AMedievalTDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMedievalTDGameModeBase();
	
	virtual void BeginPlay() override;
	
	void TickWave(float DeltaSeconds);

	void StartNextWave();
	
	void TickBetweenWave(float DeltaSeconds);
	
	virtual void Tick(float DeltaSeconds) override;

	void SpawnRandomMob();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* CrystalPedestalClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeBetweenWaves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWaveDefinition> WaveDefinitions;
	
	UPROPERTY(BlueprintReadOnly)
	float TimeBeforeNextWave = 10;

	UPROPERTY(BlueprintReadOnly)
	int32 WaveIndex;

	UPROPERTY(BlueprintReadOnly)
	int32 IsWaveOngoing;

	UPROPERTY(BlueprintReadOnly)
	int32 WaveRemainingMonsters;

	UPROPERTY(BlueprintReadOnly)
	int32 WaveTotalMonsters;

	UPROPERTY(BlueprintReadOnly)
	int32 WaveSpawnedMonsters;
	
	UPROPERTY(BlueprintReadOnly)
	FVector CrystalPosition;

	FWaveDefinition* CurrentWaveDefinition;

	TMap<TSubclassOf<AMonsterAI>, int> RemainingMobToSpawn;

	float SpawnMobInterval;
	float SpawnMobTimer;
};
