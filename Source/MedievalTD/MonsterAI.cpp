// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI.h"
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

void AMonsterAI::OnBuildingCollisionStart()
{
	MonsterState = ATTACK;
	VelocityMagnitude = 0;
	m_attackTimer = -1;
}

void AMonsterAI::OnBuildingCollisionEnd()
{
	MonsterState = RUN;
}

void AMonsterAI::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Atacc"));
}

