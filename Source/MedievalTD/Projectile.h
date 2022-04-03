// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterAI.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class MEDIEVALTD_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY(BlueprintReadOnly)
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintReadOnly)
	int Damage;
	
	UPROPERTY(BlueprintReadOnly)
	float ImpactRadius;

	UPROPERTY(BlueprintReadOnly)
	bool CanShootGround;
	
	UPROPERTY(BlueprintReadOnly)
	bool CanShootAir;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FVector LaunchToTarget(AMonsterAI& target);

	void OnImpact(AActor* OtherActor);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnImpactBP();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnImpactWithMob(AMonsterAI* monster);

	UFUNCTION(BlueprintImplementableEvent)
	void OnImpactWithGround();
};
