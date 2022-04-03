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
	ATTACK
};

UCLASS()
class MEDIEVALTD_API AMonsterAI : public AActor, public IHealth
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMonsterAI();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFlying = false;

	UPROPERTY(BlueprintReadOnly)
		float VelocityMagnitude;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* SkMeshComponent;

	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte<EMonsterState> MonsterState = RUN;

	float GetHealth_Implementation() override;
	float GetBaseHealth_Implementation() override;

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

private:

	float BaseHealth = 100;
	float m_attackInterval = 1.0f;
	float m_attackTimer;
	ABuilding* m_currentBuilding;

	void Attack();
};
