// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TextFX.generated.h"

UCLASS()
class MEDIEVALTD_API ATextFX : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATextFX();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnTextSet(const FString& text);
};
