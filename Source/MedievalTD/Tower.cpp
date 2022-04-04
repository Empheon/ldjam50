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

	TimeBeforeNextShoot = 0.3f;
}

void ATower::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TimeBeforeNextShoot -= DeltaSeconds;
	if(TimeBeforeNextShoot <= 0)
	{
		if(TryShoot())
		{
			TimeBeforeNextShoot = LevelDefinitions[Level].ShootCooldownDuration;
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
		if(mob && !mob->IsDead)
		{
			float dist = (pos - mob->GetActorLocation()).SizeSquared();
			if(dist < minDist)
			{
				minDist = dist;
				minMonster = mob;
			}
		}
	}

	if(minMonster && LevelDefinitions[Level].ProjectileClass)
	{
		const FVector spawnPoint = GetActorTransform().TransformPosition(ProjectileSpawnLocation);
		AProjectile* Projectile = Cast<AProjectile>(GetWorld()->SpawnActor(LevelDefinitions[Level].ProjectileClass, &spawnPoint));
		if(Projectile)
		{
			FVector dir = Projectile->LaunchToTarget(*minMonster);
			Projectile->Damage = LevelDefinitions[Level].Damage;
			Projectile->ImpactRadius = LevelDefinitions[Level].ImpactRadius;
			Projectile->CanShootAir = CanShootAir;
			Projectile->CanShootGround = CanShootGround;
			OnShoot(dir);
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
			if((Monster->IsFlying && CanShootAir) || (!Monster->IsFlying && CanShootGround))
			{
				MonstersInRange.Add(Monster);
			}
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
			if(((Monster->IsFlying && CanShootAir) || (!Monster->IsFlying && CanShootGround)))
			{
				MonstersInRange.Remove(Monster);
			}
		}
	}
}