// Fill out your copyright notice in the Description page of Project Settings.


#include "MedievalTDPlayerController.h"
#include "Building.h"
#include "generator.h"
#include "tower.h"

void AMedievalTDPlayerController::OnPlaceBuildingPressed(int x, int y)
{
	if (this->SelectedBuilding.Building == NULL)
		return;

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
