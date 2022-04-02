// Copyright Epic Games, Inc. All Rights Reserved.


#include "MedievalTDGameModeBase.h"

void AMedievalTDGameModeBase::AddBuilding(ABuilding* building) {
	this->Buildings.Add(building);

	auto increase = 0;
	if (this->PriceIncreaseMap.Contains(building->GetClass()))
		increase = *(this->PriceIncreaseMap.Find(building->GetClass()));

	if (this->PriceMap.Contains(building->GetClass())) {
		auto currentPrice = *(this->PriceMap.Find(building->GetClass()));
		this->PriceMap.Add(building->GetClass(), currentPrice + increase);
	}
}

void AMedievalTDGameModeBase::RemoveBuilding(ABuilding* building) {
	this->Buildings.Remove(building);

	auto decrease = 0;
	if (this->PriceIncreaseMap.Contains(building->GetClass()))
		decrease = *(this->PriceIncreaseMap.Find(building->GetClass()));

	if (this->PriceMap.Contains(building->GetClass())) {
		auto currentPrice = *(this->PriceMap.Find(building->GetClass()));
		this->PriceMap.Add(building->GetClass(), currentPrice - decrease);
	}
}

// Called when the game starts or when spawned
void AMedievalTDGameModeBase::BeginPlay() {
	Super::BeginPlay();
}