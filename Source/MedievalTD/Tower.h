// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building.h"
#include "MonsterAI.h"
#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Tower.generated.h"

/**
 * 
 */
UCLASS()
class MEDIEVALTD_API ATower : public ABuilding
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);
	ATower();
	
protected:
	virtual void BeginPlay() override;

	bool TryShoot();
	
	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ProjectileSpawnLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootCooldownDuration;
	
	UPROPERTY(BlueprintReadOnly)
	float TimeBeforeNextShoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Damage;

	UFUNCTION(BlueprintImplementableEvent)
	void OnShoot(FVector direction);
private:
	TArray<AMonsterAI*> MonstersInRange;
};
