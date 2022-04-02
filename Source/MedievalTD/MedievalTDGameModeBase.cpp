// Copyright Epic Games, Inc. All Rights Reserved.

#include "MedievalTDGameModeBase.h"
#include "GameFramework/GameSession.h"

void AMedievalTDGameModeBase::AddBuilding(ABuilding* building)
{
	this->Buildings.Add(building);

	auto increase = 0;
	if (this->PriceIncreaseMap.Contains(building->GetClass()))
		increase = *(this->PriceIncreaseMap.Find(building->GetClass()));

	if (this->PriceMap.Contains(building->GetClass()))
	{
		auto currentPrice = *(this->PriceMap.Find(building->GetClass()));
		this->PriceMap.Add(building->GetClass(), currentPrice + increase);
	}
}

void AMedievalTDGameModeBase::RemoveBuilding(ABuilding* building)
{
	this->Buildings.Remove(building);

	auto decrease = 0;
	if (this->PriceIncreaseMap.Contains(building->GetClass()))
		decrease = *(this->PriceIncreaseMap.Find(building->GetClass()));

	if (this->PriceMap.Contains(building->GetClass()))
	{
		auto currentPrice = *(this->PriceMap.Find(building->GetClass()));
		this->PriceMap.Add(building->GetClass(), currentPrice - decrease);
	}
}

AMedievalTDGameModeBase::AMedievalTDGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMedievalTDGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	AActor* playerStart = FindPlayerStart(PlayerController);

	if (playerStart)
	{
		CrystalPosition = playerStart->GetActorLocation();
	}
	else if (PlayerController)
	{
		CrystalPosition = PlayerController->GetSpawnLocation();
	}

	if (CrystalPedestalClass)
	{
		GetWorld()->SpawnActor(CrystalPedestalClass, &CrystalPosition);
	}

	TimeBeforeNextWave = TimeBetweenWaves;
}

void AMedievalTDGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsWaveOngoing)
	{
		TickWave(DeltaSeconds);
	}
	else
	{
		TickBetweenWave(DeltaSeconds);
	}
}

void AMedievalTDGameModeBase::TickWave(float DeltaSeconds)
{
	if (WaveSpawnedMonsters < WaveTotalMonsters)
	{
		SpawnMobTimer += DeltaSeconds;
		while (SpawnMobTimer >= SpawnMobInterval)
		{
			SpawnRandomMob();
			SpawnMobTimer -= SpawnMobInterval;
		}
	}
}

void AMedievalTDGameModeBase::TickBetweenWave(float DeltaSeconds)
{
	TimeBeforeNextWave -= DeltaSeconds;

	if (TimeBeforeNextWave <= 0.0f)
	{
		StartNextWave();
	}
}

void AMedievalTDGameModeBase::StartNextWave()
{
	IsWaveOngoing = true;

	CurrentWaveDefinition = &WaveDefinitions[FMath::Min(WaveIndex, WaveDefinitions.Num() - 1)];
	WaveRemainingMonsters = 0;

	RemainingMobToSpawn.Empty();

	for (auto MobCount : CurrentWaveDefinition->MobCount)
	{
		WaveRemainingMonsters += MobCount.Value;
		RemainingMobToSpawn.Add(MobCount.Key, MobCount.Value);
	}

	SpawnMobInterval = CurrentWaveDefinition->TotalSpawnTime / WaveRemainingMonsters;
	SpawnMobTimer = 0.0f;
	WaveSpawnedMonsters = 0;
	WaveTotalMonsters = WaveRemainingMonsters;
}

void AMedievalTDGameModeBase::SpawnRandomMob()
{
	const int32 count = RemainingMobToSpawn.Num();

	const int32 indexToPickFrom = FMath::RandRange(0, count);

	TSubclassOf<AMonsterAI> pickedMob = nullptr;
	int i = 0;
	for (const auto MobToSpawn : RemainingMobToSpawn)
	{
		if (i == indexToPickFrom)
		{
			pickedMob = MobToSpawn.Key;
		}
	}

	if (pickedMob)
	{
		int32 remainingMob = RemainingMobToSpawn[pickedMob];
		remainingMob--;

		if (remainingMob > 0)
		{
			RemainingMobToSpawn[pickedMob] = remainingMob;
		}
		else
		{
			RemainingMobToSpawn.Remove(pickedMob);
		}

		const FVector spawnLocation = CrystalPosition - FVector(1000, FMath::RandRange(-300, 300), 0);
		const FRotator rot = (CrystalPosition - spawnLocation).GetSafeNormal().Rotation();
		AMonsterAI* MonsterAI = Cast<AMonsterAI>(GetWorld()->SpawnActor(pickedMob, &spawnLocation, &rot));
		MonsterAI->TargetLocation = CrystalPosition;
	}
}