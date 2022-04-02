// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MedievalTDPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MEDIEVALTD_API AMedievalTDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	public:
		UFUNCTION(BlueprintCallable)
		void OnPlaceBuildingPressed(UClass* actor, int x, int y);

};
