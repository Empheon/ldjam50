// Fill out your copyright notice in the Description page of Project Settings.


#include "MedievalTDPlayerController.h"
#include "Building.h"
#include "generator.h"
#include "tower.h"

void AMedievalTDPlayerController::OnPlaceBuildingPressed(int x, int y, TArray<AActor*> collisions)
{
	if (this->SelectedBuilding.Building == NULL)
		return;

	 UE_LOG(LogTemp, Warning, TEXT("A"));
	if (this->SelectedBuilding.RequiredActor != NULL) {
		if (!this->RequiredActorExists(this->SelectedBuilding.RequiredActor, collisions))
			return;
	} else if (collisions.Num() > 1) {
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("B"));

	if (this->SelectedBuilding.Price > this->Money)
		return;

	this->Money -= this->SelectedBuilding.Price;

	const FRotator myRot(0, 0, 0);
	const FVector myLoc(x, y, 0);

	GetWorld()->SpawnActor(this->SelectedBuilding.Building, &myLoc, &myRot);
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
