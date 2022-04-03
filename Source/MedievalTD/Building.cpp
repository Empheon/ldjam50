// Fill out your copyright notice in the Description page of Project Settings.


#include "MedievalTDGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Building.h"

// Sets default values
ABuilding::ABuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuilding::Destroy()
{
	auto gameMode = Cast<AMedievalTDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->RemoveBuilding(this);
}

void ABuilding::Upgrade()
{
	if (this->Level >= this->LevelMeshes.Num())
		return;
	this->Level++;
	auto rootComponent = Cast<UStaticMeshComponent>(this->RootComponent);
	rootComponent->SetStaticMesh(this->LevelMeshes[this->Level++]);
}