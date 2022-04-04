// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterAI.h"

#include "Building.h"
#include "MedievalTDGameModeBase.h"
#include "MedievalTDPlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMonsterAI::AMonsterAI()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SkMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MonsterMesh"));
    SkMeshComponent->SetEnableGravity(false);
    RootComponent = SkMeshComponent;
}

// Called when the game starts or when spawned
void AMonsterAI::BeginPlay()
{
    Super::BeginPlay();
	SetActorScale3D(GetActorScale3D() * 0.7f);
}

// Called every frame
void AMonsterAI::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsFlying)
    {
        TargetLocation.Z = 250;
        FVector pos = GetActorLocation();
        pos.Z = 250;
        SetActorLocation(pos);
    }

    FRotator targetRotation;

    if (MonsterState == RUN)
    {
        targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);

		FVector newPos = FMath::VInterpConstantTo(GetActorLocation(), TargetLocation, DeltaTime, Speed);
    	float displacement = (GetActorLocation() - newPos).Size();
    	ActualVelocityMagnitude = displacement / DeltaTime;
		VelocityMagnitude = displacement * Speed;
	
		SetActorLocation(newPos);
	}

    if (MonsterState == ATTACK)
    {
        m_attackTimer -= DeltaTime;

        if (m_attackTimer < 0)
        {
            m_attackTimer = m_attackInterval;

            Attack();
        }

        FVector buildingLocation = m_currentBuilding->GetActorLocation();
        buildingLocation.Z = IsFlying ? 250 : buildingLocation.Z;
        targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), buildingLocation);
    }

    SetActorRotation(FMath::RInterpTo(GetActorRotation(), targetRotation, DeltaTime, 5));
}

void AMonsterAI::OnBuildingCollisionStart(ABuilding *building)
{
    MonsterState = ATTACK;
    VelocityMagnitude = 0;
    m_attackTimer = -1;
    m_currentBuilding = building;
}

void AMonsterAI::OnBuildingCollisionEnd()
{
    MonsterState = RUN;
}

void AMonsterAI::TakeHit(float damage)
{
	Health -= damage;
	if (Health <= 0)
	{
		AMedievalTDGameModeBase* GameModeBase = Cast<AMedievalTDGameModeBase>(GetWorld()->GetAuthGameMode());
		if(GameModeBase)
		{
			GameModeBase->WaveRemainingMonsters--;
		}
		
	    AMedievalTDPlayerController* pc = Cast<AMedievalTDPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	    if(pc)
	    {
	    	pc->Money += MoneyGiven;
	    }
		Destroy();
	}
}

void AMonsterAI::SetLevel(int level)
{
	Level = level;

	if (level < LevelInfos.Num())
	{
		FLevelInfo levelInfo = LevelInfos[Level];
		
		SkMeshComponent->SetMaterial(0, levelInfo.Material);
		Health = levelInfo.Health;
		Speed = levelInfo.Speed;
		Damage = levelInfo.Damage;
		MoneyGiven = levelInfo.MoneyGiven;
	}

	
	this->BaseHealth = this->Health;
}

void AMonsterAI::Attack()
{
    if (!IsValid(m_currentBuilding))
    {
        return;
    }

    m_currentBuilding->TakeHit(Damage);
}

float AMonsterAI::GetHealth_Implementation()
{
    return this->Health;
}

float AMonsterAI::GetBaseHealth_Implementation()
{
    return this->BaseHealth;
}
