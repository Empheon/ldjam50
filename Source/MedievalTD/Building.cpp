// Fill out your copyright notice in the Description page of Project Settings.

#include "Building.h"
#include "MedievalTDGameModeBase.h"
#include "Kismet/GameplayStatics.h"

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
	AActor::Destroy();
}

void ABuilding::Upgrade()
{
    if (this->Level >= this->LevelMeshes.Num() - 1)
        return;
    this->Level++;
    TArray<USceneComponent*> children;
    this->RootComponent->GetChildrenComponents(true, children);
    for (auto i = 0; i < children.Num(); i++) {
        auto component = Cast<UStaticMeshComponent>(children[i]);
        if (component) {
            component->SetStaticMesh(this->LevelMeshes[this->Level]);
            break;
        }
    }
}

void ABuilding::TakeHit(float damage)
{
    Health -= damage;
    if (Health <= 0)
    {
        Destroy();
    }
}

float ABuilding::GetHealth_Implementation() {
    return this->Health;
}