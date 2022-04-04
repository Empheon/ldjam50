// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildLevelDefinition.generated.h"

class AProjectile;

USTRUCT(BlueprintType)
struct FBuildLevelDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* LevelMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NextUpgradeCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ImpactRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootCooldownDuration;
};