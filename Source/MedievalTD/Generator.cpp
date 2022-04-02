// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"
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

	if (this->timer < 1)
		return;
	if (this->pc == NULL)
		return;
	
	this->pc->Money += 1 * (this->Level + 1);
	this->timer = 0;
}
