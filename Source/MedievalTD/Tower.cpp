// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

#include "MonsterAI.h"
#include "Components/SphereComponent.h"

ATower::ATower()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionRadius"));
	SphereComponent->InitSphereRadius(15.0f);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetupAttachment(RootComponent);
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATower::OnOverlapBegin); 
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ATower::OnOverlapEnd); 
}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	TimeBeforeNextShoot = ShootCooldownDuration;
}

void ATower::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TimeBeforeNextShoot -= DeltaSeconds;
	if(TimeBeforeNextShoot <= 0)
	{
		if(TryShoot())
		{
			TimeBeforeNextShoot = ShootCooldownDuration;
		}
	}
}

bool ATower::TryShoot()
{
	FVector pos = GetActorLocation();
	float minDist = TNumericLimits<float>::Max();
	AMonsterAI* minMonster = nullptr;
	for (AMonsterAI* mob : MonstersInRange)
	{
		if(mob)
		{
			float dist = (pos - mob->GetActorLocation()).SizeSquared();
			if(dist < minDist)
			{
				minDist = dist;
				minMonster = mob;
			}
		}
	}

	if(minMonster && ProjectileClass)
	{
		const FVector spawnPoint = GetActorTransform().TransformPosition(ProjectileSpawnLocation);
		AProjectile* Projectile = Cast<AProjectile>(GetWorld()->SpawnActor(ProjectileClass, &spawnPoint));
		if(Projectile)
		{
			Projectile->LaunchToTarget(*minMonster);
			return true;
		}
		
		return false;
	}

	return false;
}

void ATower::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) 
	{
		AMonsterAI* Monster = Cast<AMonsterAI>(OtherActor);
		if(Monster)
		{
			MonstersInRange.Add(Monster);
		}
	}
} 

void ATower::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp) 
	{
		AMonsterAI* Monster = Cast<AMonsterAI>(OtherActor);
		if(Monster)
		{
			MonstersInRange.Remove(Monster);
		}
	}
}