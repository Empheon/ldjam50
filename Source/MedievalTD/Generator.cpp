// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"

#include "MedievalTDGameModeBase.h"
#include "MedievalTDPlayerController.h"

void AGenerator::BeginPlay()
{
	Super::BeginPlay();

	for (FConstPlayerControllerIterator iterator = GetWorld()->GetPlayerControllerIterator(); iterator; ++iterator)
	{
		//AMedievalTDPlayerController* pc = Cast<AMedievalTDPlayerController>(*iterator);
		this->pc = Cast<AMedievalTDPlayerController>(*iterator);
		if (pc != NULL)
			return;
	}
}

// Called every frame
void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	this->timer += DeltaTime;

	// https://unreal.blog/how-to-print-strings-to-console-or-screen-in-unreal-engine-c-at-runtime
	// UE_LOG(LogTemp, Warning, TEXT("Output: %f"), this->timer);

	if (this->timer < GenerateInterval)
		return;
	if (this->pc == NULL)
		return;

	AMedievalTDGameModeBase* GameModeBase = Cast<AMedievalTDGameModeBase>(GetWorld()->GetAuthGameMode());
	if(GameModeBase)
	{
		int reward = CoinEarned + GameModeBase->WaveIndex * AdditionalCoinPerWave;
		this->pc->Money += reward;
		this->timer = 0;
		
		auto text = FString::Printf(TEXT("+%d"), reward);
		GameModeBase->SpawnTextFX(text, GetActorLocation());
	}
}
