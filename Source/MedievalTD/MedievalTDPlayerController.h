// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MedievalTDPlayerController.generated.h"

USTRUCT(BlueprintType)
struct FSelectedBuilding
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UClass* Building;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* RequiredActor;
};

/**
 * 
 */
UCLASS()
class MEDIEVALTD_API AMedievalTDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSelectedBuilding SelectedBuilding;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Money = 500;
	UFUNCTION(BlueprintCallable)
		void OnPlaceBuildingPressed(int x, int y, TArray<AActor*> collisions);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool RequiredActorExists(UClass* requiredActor, TArray<AActor*> collisions);
	
};
