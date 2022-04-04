// Copyright Epic Games, Inc. All Rights Reserved.

#include "MedievalTDGameModeBase.h"

#include "TextFX.h"
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

void AMedievalTDGameModeBase::EndWave()
{
	IsWaveOngoing = false;
	WaveIndex++;
}

// Called when the game starts or when spawned
void AMedievalTDGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	for(int i = 0; i < 50; i++)
	{
		ATextFX* textFX = (ATextFX*) GetWorld()->SpawnActor(TextFXClass);
		textFX->SetHidden(true);
		TextFXPool.Add(textFX);
	}

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

	if (WaveRemainingMonsters <= 0)
	{
		EndWave();
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

	float additionalMobFactor = FMath::Max(0, WaveIndex - WaveDefinitions.Num() + 1);
	float multiplier = 1 + 0.2f * additionalMobFactor;

	for (auto MobInfo : CurrentWaveDefinition->MobInfos)
	{
		WaveRemainingMonsters += MobInfo.Count * multiplier;

		FMobInfo newMobInfo = MobInfo;
		newMobInfo.Count = FMath::RoundToInt(newMobInfo.Count * multiplier);
		newMobInfo.Level = FMath::RoundToInt(newMobInfo.Level * (1 + 0.4f * additionalMobFactor));
		newMobInfo.Level = FMath::Clamp(newMobInfo.Level, 0, 2);

		RemainingMobToSpawn.Add(newMobInfo);
	}

	SpawnMobInterval = CurrentWaveDefinition->TotalSpawnTime / WaveRemainingMonsters;
	SpawnMobTimer = 0.0f;
	WaveSpawnedMonsters = 0;
	WaveTotalMonsters = WaveRemainingMonsters;
}

void AMedievalTDGameModeBase::SpawnRandomMob()
{
	const int32 count = RemainingMobToSpawn.Num();

	if (count == 0)
	{
		return;
	}

	const int32 indexToPickFrom = FMath::RandRange(0, count - 1);

	FMobInfo pickedMob = RemainingMobToSpawn[indexToPickFrom];


	int32 remainingMob = pickedMob.Count;
	remainingMob--;

	if (remainingMob > 0)
	{
		RemainingMobToSpawn[indexToPickFrom].Count = remainingMob;
	}
	else
	{
		RemainingMobToSpawn.Remove(pickedMob);
	}

	ESpawnSide side = BOTTOM;
	if (CurrentWaveDefinition->EnabledSpawnSides.Num() > 0)
	{
		const int32 randomSide = FMath::RandRange(0, CurrentWaveDefinition->EnabledSpawnSides.Num() - 1);
		side = CurrentWaveDefinition->EnabledSpawnSides[randomSide];
	}

	const float horizontal = 5000;
	const float vertical = 4000;
	FVector spawnLocation;
	switch (side)
	{
	case TOP:
		{
			spawnLocation = CrystalPosition + FVector(2800, FMath::RandRange(-horizontal, horizontal), 0);
			break;
		}
	case BOTTOM:
		{
			spawnLocation = CrystalPosition - FVector(2800, FMath::RandRange(-horizontal, horizontal), 0);
			break;
		}
	case LEFT:
		{
			spawnLocation = CrystalPosition - FVector(FMath::RandRange(-vertical, vertical), 3900, 0);
			break;
		}
	case RIGHT:
		{
			spawnLocation = CrystalPosition + FVector(FMath::RandRange(-vertical, vertical), 3900, 0);
			break;
		}
	default: ;
	}

	const FRotator rot = (CrystalPosition - spawnLocation).GetSafeNormal().Rotation();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.bNoFail = true;
	AMonsterAI* MonsterAI = Cast<AMonsterAI>(
		GetWorld()->SpawnActor(pickedMob.MobClass, &spawnLocation, &rot, SpawnParameters));
	MonsterAI->SetLevel(pickedMob.Level);
	MonsterAI->TargetLocation = CrystalPosition;
}

void AMedievalTDGameModeBase::SpawnTextFX(FString& text, FVector pos)
{
	for (ATextFX* FXPool : TextFXPool)
	{
		if(FXPool->IsHidden())
		{
			FXPool->SetActorLocation(pos);
			FXPool->SetHidden(false);
			FXPool->OnTextSet(text);
			break;
		}
	}
}