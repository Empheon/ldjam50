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
}

// Called every frame
void AMonsterAI::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
	if (IsDead)
	{
		return;
	}

	FVector pos = GetActorLocation();
    if (IsFlying)
    {
        TargetLocation.Z = 250;
        
        pos.Z = 250;
        SetActorLocation(pos);
    }

    FRotator targetRotation;

    if (MonsterState == RUN)
    {
        targetRotation = UKismetMathLibrary::FindLookAtRotation(pos, TargetLocation);

		FVector newPos = FMath::VInterpConstantTo(pos, TargetLocation, DeltaTime, Speed);
    	float displacement = (pos - newPos).Size();
    	ActualVelocityMagnitude = Speed;
		VelocityMagnitude = displacement * Speed;
	
		SetActorLocation(newPos);
	}

    if (MonsterState == ATTACK)
    {
        m_attackTimer -= DeltaTime;
    	ActualVelocityMagnitude = 0;
    	
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
	if (IsDead)
	{
		return;
	}
    MonsterState = ATTACK;
    VelocityMagnitude = 0;
	ActualVelocityMagnitude = 0;
    m_attackTimer = -1;
    m_currentBuilding = building;
}

void AMonsterAI::OnBuildingCollisionEnd()
{
	if (IsDead)
	{
		return;
	}
    MonsterState = RUN;
}

void AMonsterAI::TakeHit(float damage)
{
	if(IsDead)
	{
		return;
	}
	
	Health -= damage;
	if (Health <= 0)
	{
		IsDead = true;
		MonsterState = DEAD;
		VelocityMagnitude = 0;
		
		AMedievalTDGameModeBase* GameModeBase = Cast<AMedievalTDGameModeBase>(GetWorld()->GetAuthGameMode());
		if(GameModeBase)
		{
			GameModeBase->WaveRemainingMonsters--;
			auto text = FString::Printf(TEXT("+%d"), MoneyGiven);
			GameModeBase->SpawnTextFX(text, GetActorLocation());
		}
		
	    AMedievalTDPlayerController* pc = Cast<AMedievalTDPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	    if(pc)
	    {
	    	pc->Money += MoneyGiven;
	    }
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

	SetActorScale3D(GetActorScale3D() * LevelInfos[Level].Scale);

	
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

void AMonsterAI::DestroyAux()
{
	Destroy();
}

float AMonsterAI::GetHealth_Implementation()
{
    return this->Health;
}

float AMonsterAI::GetBaseHealth_Implementation()
{
    return this->BaseHealth;
}
