// Fill out your copyright notice in the Description page of Project Settings.

#include "Building.h"
#include "MedievalTDGameModeBase.h"
#include "MedievalTDPlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABuilding::ABuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp"));

	const ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("CurveFloat'/Game/Resources/BuildSpawnCurve'"));
	if(Curve.Succeeded())
	{
		SpawnTimelineFloatCurve = Curve.Object;
	}
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	if(LevelDefinitions.Num() > 0)
	{
		MaxHealth = Health = LevelDefinitions[0].Health;
	}
	else
	{
		MaxHealth = Health = 100;
	}

	UpdateSpawnTimelineFunction.BindDynamic(this, &ABuilding::UpdateTimelineComp);

	//If we have a float curve, bind it's graph to our update functionq
	if (SpawnTimelineFloatCurve)
	{

		TimelineComp->AddInterpFloat(SpawnTimelineFloatCurve, UpdateSpawnTimelineFunction);
	}

	TimelineComp->Play();
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Health = FMath::Min(MaxHealth, Health + HealthRegenPerSecond * DeltaTime);
}

void ABuilding::Destroy()
{
	auto gameMode = Cast<AMedievalTDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->RemoveBuilding(this);
	AActor::Destroy();
}

void ABuilding::Upgrade()
{
	if (this->Level >= this->LevelDefinitions.Num() - 1)
		return;
	

	auto currentPrice = LevelDefinitions[Level].NextUpgradeCost;
	AMedievalTDPlayerController* pc = Cast<AMedievalTDPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if(pc && pc->Money >= currentPrice)
	{
		pc->Money -= currentPrice;
		
		this->Level++;
		
		float maxHealth = LevelDefinitions[Level].Health;
		float percentUpgrade = maxHealth / MaxHealth;
		Health *= percentUpgrade;
		MaxHealth = maxHealth;
		
		TArray<USceneComponent*> children;
		this->RootComponent->GetChildrenComponents(true, children);
		for (auto i = 0; i < children.Num(); i++) {
			auto component = Cast<UStaticMeshComponent>(children[i]);
			if (component) {
				component->SetStaticMesh(this->LevelDefinitions[this->Level].LevelMesh);
				break;
			}
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


void ABuilding::UpdateTimelineComp(float Output)
{
	FVector ActorLocation = GetActorLocation();
	ActorLocation[2] = Output;
	SetActorLocation(ActorLocation);
}

float ABuilding::GetHealth_Implementation() {
	return this->Health;
}

float ABuilding::GetBaseHealth_Implementation() {
	return this->MaxHealth;
}