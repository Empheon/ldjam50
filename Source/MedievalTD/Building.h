// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildLevelDefinition.h"
#include "Health.h"
#include "GameFramework/Actor.h"
#include "BuildLevelDefinition.h"
#include "Components/TimelineComponent.h"
#include "Components/TimelineComponent.h"
#include "Building.generated.h"

/**
 *
 */
UCLASS()
class MEDIEVALTD_API ABuilding : public AActor, public IHealth
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FBuildLevelDefinition> LevelDefinitions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Level = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Popup = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float HealthRegenPerSecond = 1;
	UFUNCTION(BlueprintCallable)
		virtual void Destroy();
	UFUNCTION(BlueprintCallable)
		virtual void Upgrade();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UTimelineComponent* TimelineComp;
	UPROPERTY(EditAnywhere)
		UCurveFloat* SpawnTimelineFloatCurve;

	
	float GetHealth_Implementation() override;
	float GetBaseHealth_Implementation() override;
	void TakeHit(float damage);

	// Sets default values for this actor's properties
	ABuilding();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	float MaxHealth = 100;

	FOnTimelineFloat UpdateSpawnTimelineFunction;

	UFUNCTION()
	void UpdateTimelineComp(float Output);
};
