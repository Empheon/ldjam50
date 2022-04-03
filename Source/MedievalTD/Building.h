// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Health.h"
#include "GameFramework/Actor.h"
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
		TArray<UStaticMesh*> LevelMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Level = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Popup = true;
	UFUNCTION(BlueprintCallable)
		virtual void Destroy();
	UFUNCTION(BlueprintCallable)
		virtual void Upgrade();
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
	float BaseHealth = 100;
};
