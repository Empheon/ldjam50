// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building.h"
#include "Health.h"
#include "GameFramework/Actor.h"
#include "MonsterAI.generated.h"


UENUM(BlueprintType)
enum EMonsterState
{
	RUN,
	ATTACK,
	DEAD
};

USTRUCT(BlueprintType)
struct FLevelInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MoneyGiven = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Scale = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* Material;
};

UCLASS()
class MEDIEVALTD_API AMonsterAI : public AActor, public IHealth
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMonsterAI();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLevelInfo> LevelInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MoneyGiven = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFlying = false;

	UPROPERTY(BlueprintReadOnly)
		float VelocityMagnitude;

	UPROPERTY(BlueprintReadOnly)
	float ActualVelocityMagnitude;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* SkMeshComponent;

	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte<EMonsterState> MonsterState = RUN;

	float GetHealth_Implementation() override;
	float GetBaseHealth_Implementation() override;
	bool IsDead;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void OnBuildingCollisionStart(ABuilding* building);

	UFUNCTION(BlueprintCallable)
		void OnBuildingCollisionEnd();

	void TakeHit(float damage);

	void SetLevel(int level);

	UFUNCTION(BlueprintCallable)
	void DestroyAux();

private:

	float BaseHealth = 100;
	int Level;
	float m_attackInterval = 1.0f;
	float m_attackTimer;
	ABuilding* m_currentBuilding;

	void Attack();
};
