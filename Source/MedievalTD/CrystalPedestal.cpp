// Fill out your copyright notice in the Description page of Project Settings.


#include "CrystalPedestal.h"

#include "Kismet/GameplayStatics.h"

void ACrystalPedestal::Destroy()
{
	if(!GameOverWidget)
	{
		return;
	}
	
	APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	CreateWidget<UUserWidget>(pc, GameOverWidget)->AddToViewport();
	pc->SetPause(true);
	
	Super::Destroy();
}
