// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterAI.generated.h"



enum MonsterState
{
	RUN,
	ATTACK
};

UCLASS()
class MEDIEVALTD_API AMonsterAI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterAI();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 100.0f;

	UPROPERTY(BlueprintReadOnly)
	float VelocityMagnitude;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkMeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnBuildingCollisionStart();
	
	UFUNCTION(BlueprintCallable)
	void OnBuildingCollisionEnd();

private:
	MonsterState m_monsterState = RUN;

	float m_attackInterval = 1.0f;
	float m_attackTimer;

	void Attack();
};
