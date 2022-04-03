// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI.h"

#include "Building.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMonsterAI::AMonsterAI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MonsterMesh"));
	RootComponent = SkMeshComponent;
}

// Called when the game starts or when spawned
void AMonsterAI::BeginPlay()
{
	Super::BeginPlay();
	this->BaseHealth = this->Health;
}

// Called every frame
void AMonsterAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MonsterState == RUN)
	{
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation));

		FVector newPos = FMath::VInterpConstantTo(GetActorLocation(), TargetLocation, DeltaTime, Speed);
		VelocityMagnitude = (GetActorLocation() - newPos).Size() * Speed;
	
		SetActorLocation(newPos, true);
	}

	if (MonsterState == ATTACK)
	{
		m_attackTimer -= DeltaTime;

		if (m_attackTimer < 0)
		{
			m_attackTimer = m_attackInterval;

			Attack();
		}
	}
}

void AMonsterAI::OnBuildingCollisionStart(ABuilding* building)
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
		Destroy();
	}
}

void AMonsterAI::Attack()
{
	if (!IsValid(m_currentBuilding))
	{
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Atacc"));
	m_currentBuilding->TakeHit(Damage);
}

float AMonsterAI::GetHealth_Implementation() {
	return this->Health;
}

float AMonsterAI::GetBaseHealth_Implementation() {
	return this->BaseHealth;
}
