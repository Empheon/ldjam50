// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Health.generated.h"

UINTERFACE(BlueprintType)
class UHealth : public UInterface
{
	GENERATED_BODY()
};

class IHealth
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float GetHealth();
};