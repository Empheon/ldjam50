// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Building.h"
#include "FWaveDefinition.h"
#include "GameFramework/GameModeBase.h"
#include "MedievalTDGameModeBase.generated.h"

class ATextFX;
/**
 *
 */
UCLASS()
class MEDIEVALTD_API AMedievalTDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	AMedievalTDGameModeBase();

	void EndWave();
	void TickWave(float DeltaSeconds);

	void StartNextWave();

	void TickBetweenWave(float DeltaSeconds);

	virtual void Tick(float DeltaSeconds) override;

	void SpawnRandomMob();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TSubclassOf<ABuilding>, int> PriceMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TSubclassOf<ABuilding>, int> PriceIncreaseMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ABuilding*> Buildings;
	UPROPERTY(BlueprintReadOnly)
		TArray<ATextFX*> TextFXPool;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ATextFX> TextFXClass;
	UFUNCTION(BlueprintCallable)
		void AddBuilding(ABuilding* building);
	UFUNCTION(BlueprintCallable)
		void RemoveBuilding(ABuilding* building);

	UFUNCTION(BlueprintCallable)
	void SpawnTextFX(FString& text, FVector pos);

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
		bool IsWaveOngoing;

	UPROPERTY(BlueprintReadOnly)
		int32 WaveRemainingMonsters;

	UPROPERTY(BlueprintReadOnly)
		int32 WaveTotalMonsters;

	UPROPERTY(BlueprintReadOnly)
		int32 WaveSpawnedMonsters;

	UPROPERTY(BlueprintReadOnly)
		FVector CrystalPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* StartWaveSound;

	FWaveDefinition* CurrentWaveDefinition;

	TArray<FMobInfo> RemainingMobToSpawn;

	float SpawnMobInterval;
	float SpawnMobTimer;
};
