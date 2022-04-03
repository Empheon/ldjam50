// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"


// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	RootComponent = CollisionComponent;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovement->SetUpdatedComponent(CollisionComponent);
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->Bounciness = 0.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->Friction = 0.0f;
	ProjectileMovement->Buoyancy = 0.0f;
	
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetActorLocation().Z < -5)
	{
		Destroy();
	}
}

void AProjectile::LaunchToTarget(AMonsterAI& target)
{
	FVector projectileStartPoint = GetActorLocation();
	
	FVector enemyStartPoint = target.GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);
	FVector enemyVelocity = target.GetActorForwardVector() * target.VelocityMagnitude;
	FVector enemyVelocitySqr = enemyVelocity * enemyVelocity;
	FVector enemyStartPointSqr = enemyStartPoint * enemyStartPoint;
	FVector projectileStartPointSqr = projectileStartPoint * projectileStartPoint;

	
	float a = (enemyVelocitySqr[0] + enemyVelocitySqr[1] + enemyVelocitySqr[2]) - (ProjectileMovement->MaxSpeed * ProjectileMovement->MaxSpeed);
	float b = 2 * (enemyStartPoint[0] * enemyVelocity[0] + enemyStartPoint[1] * enemyVelocity[1] + enemyStartPoint[2] * enemyVelocity[2]) - (projectileStartPoint[0] * enemyVelocity[0]) - projectileStartPoint[1] * enemyVelocity[1] - projectileStartPoint[2] * enemyVelocity[2];
	float c = (enemyStartPointSqr[0]) + (enemyStartPointSqr[1]) + (enemyStartPointSqr[2]) + (projectileStartPointSqr[0]) + (projectileStartPointSqr[1]) + (projectileStartPointSqr[2]) -
		(2 * projectileStartPoint[0] * enemyStartPoint[0]) - (2 * projectileStartPoint[1] * enemyStartPoint[1]) - (2 * projectileStartPoint[2] * enemyStartPoint[2]);

	float d = FMath::Sqrt((b * b) - (4 * a * c));
	float e = 2 * a;
	float t1 = (-b + d) / e;
	float t2 = (-b - d) / e;
	
	FVector targetPosition;
	if (FMath::IsNaN(t1) || t1 <= 0.0f)
	{
		if (FMath::IsNaN(t2) || t2 <= 0.0f)
		{
			targetPosition = enemyStartPoint;
		}
		else
		{
			float t = t2;
			targetPosition = enemyStartPoint + (enemyVelocity * t);
		}
	}
	else if (FMath::IsNaN(t2) || t2 <= 0.0f)
	{
		float t = t1;
		targetPosition = enemyStartPoint + (enemyVelocity * t);
	}
	else
	{
		float t = FMath::Min(t1, t2);
		targetPosition = enemyStartPoint + (enemyVelocity * t);
	}
	
	FVector diff = (targetPosition - projectileStartPoint);
	ProjectileMovement->Velocity = diff.GetSafeNormal() * ProjectileMovement->MaxSpeed;
	
	DrawDebugSphere(GetWorld(), enemyStartPoint, 50, 26, FColor(181,0,0), false, 1, 0, 2);
	DrawDebugSphere(GetWorld(), targetPosition, 40, 26, FColor(0,0,181), false, 1, 0, 2);
	DrawDebugSphere(GetWorld(), projectileStartPoint, 30, 26, FColor(0,181,0), false, 1, 0, 2);
}

void AProjectile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) 
	{
		OnImpact(OtherActor);
		AMonsterAI* Monster = Cast<AMonsterAI>(OtherActor);
		if(Monster)
		{
			if(ImpactRadius <= 0)
			{
				Monster->TakeHit(Damage);
			}
			OnImpactWithMob(Monster);
			Destroy();
		}

		if(OtherComp->GetCollisionProfileName() == "Ground" || OtherComp->GetCollisionProfileName() == "Landscape")
		{
			OnImpactWithGround();
			Destroy();
		}
	}
}

void AProjectile::OnImpact(AActor* OtherActor)
{
	
}