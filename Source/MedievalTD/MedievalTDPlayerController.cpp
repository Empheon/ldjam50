// Fill out your copyright notice in the Description page of Project Settings.


#include "MedievalTDPlayerController.h"
#include "Building.h"
#include "generator.h"
#include "MedievalTDGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "tower.h"

void AMedievalTDPlayerController::OnPlaceBuildingPressed(int x, int y, TArray<AActor*> collisions)
{
	if (this->SelectedBuilding.Building == NULL)
		return;

	if (this->SelectedBuilding.RequiredActor != NULL) {
		if (!this->RequiredActorExists(this->SelectedBuilding.RequiredActor, collisions))
			return;
	}
	else if (collisions.Num() > 0) {
		return;
	}

	auto gameMode = Cast<AMedievalTDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	auto price = 0;
	if (gameMode->PriceMap.Contains(this->SelectedBuilding.Building))
		price = *(gameMode->PriceMap.Find(this->SelectedBuilding.Building));

	if (price > this->Money)
		return;

	this->Money -= price;

	const FRotator myRot(0, 0, 0);
	const FVector myLoc(x, y, 0);

	auto newActor = GetWorld()->SpawnActor(this->SelectedBuilding.Building, &myLoc, &myRot);
	auto newBuilding = (ABuilding*)newActor;
	gameMode->AddBuilding(newBuilding);

}

// Called when the game starts or when spawned
void AMedievalTDPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMedievalTDPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AMedievalTDPlayerController::RequiredActorExists(UClass* requiredActor, TArray<AActor*> collisions)
{
	for (auto& collision : collisions) {
		if (collision->IsA(requiredActor)) {
			return true;
		}
	}
	return false;
}
