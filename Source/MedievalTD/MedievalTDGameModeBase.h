// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Building.h"
#include "GameFramework/GameModeBase.h"
#include "MedievalTDGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class MEDIEVALTD_API AMedievalTDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TSubclassOf<ABuilding>, int> PriceMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TSubclassOf<ABuilding>, int> PriceIncreaseMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ABuilding*> Buildings;
	UFUNCTION(BlueprintCallable)
		void AddBuilding(ABuilding* building);
	UFUNCTION(BlueprintCallable)
		void RemoveBuilding(ABuilding* building);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
