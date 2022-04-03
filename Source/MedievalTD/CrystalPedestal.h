// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building.h"
#include "Blueprint/UserWidget.h"
#include "CrystalPedestal.generated.h"

/**
 * 
 */
UCLASS()
class MEDIEVALTD_API ACrystalPedestal : public ABuilding
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> GameOverWidget;

	virtual void Destroy() override;

	
};
