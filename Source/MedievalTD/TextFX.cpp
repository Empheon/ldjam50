// Fill out your copyright notice in the Description page of Project Settings.


#include "TextFX.h"


// Sets default values
ATextFX::ATextFX()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATextFX::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATextFX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

