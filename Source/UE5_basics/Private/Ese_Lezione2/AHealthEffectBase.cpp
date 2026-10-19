// Fill out your copyright notice in the Description page of Project Settings.

#include "UE5_basics/Public/Ese_Lezione2/AHealthEffectBase.h"

AHealthEffectBase::AHealthEffectBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AHealthEffectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHealthEffectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealthEffectBase::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
}

