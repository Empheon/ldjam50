// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building.h"
#include "MedievalTDPlayerController.h"
#include "Generator.generated.h"

/**
 * 
 */
UCLASS()
class MEDIEVALTD_API AGenerator : public ABuilding
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CoinEarned = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GenerateInterval = 1;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	AMedievalTDPlayerController* pc;
	float timer = 0;
};
