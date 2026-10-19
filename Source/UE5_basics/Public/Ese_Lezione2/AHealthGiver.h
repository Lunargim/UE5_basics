// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AHealthEffectBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "AHealthGiver.generated.h"

UCLASS()
class UE5_BASICS_API AAHealthGiver : public AHealthEffectBase
{
	GENERATED_BODY()
	
public:	
	AAHealthGiver();

private:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) override;
	
	UBoxComponent* BoxComponent;
	
	float Amount;
};