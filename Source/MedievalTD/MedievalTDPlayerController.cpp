// Fill out your copyright notice in the Description page of Project Settings.


#include "MedievalTDPlayerController.h"
#include "Building.h"

void AMedievalTDPlayerController::OnPlaceBuildingPressed(UClass* actor, int x, int y)
{

	FActorSpawnParameters SpawnInfo;

	FRotator myRot(0, 0, 0);
	const FVector myLoc(x, y, 100);

	//ABuilding* mySphere = (ABuilding*) GetWorld()->SpawnActor(ABuilding::StaticClass(), NAME_None, &myLoc);
	GetWorld()->SpawnActor(actor, &myLoc, &myRot);

}
