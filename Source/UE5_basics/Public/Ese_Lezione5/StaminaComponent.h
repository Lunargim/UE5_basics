// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStaminaDelegate);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_BASICS_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RemovingStamina(float DeltaTime);
	void RemoveStamina(float Amount);
	void EndStamina();
	void SetHasBall(bool HasBall);
	void ChangeStamina(float DeltaTime);
	void RecoveryStamina(float DeltaTime);
	void GainStamina(float Quantity);

	UPROPERTY(BlueprintAssignable)
	FStaminaDelegate OnStaminaEnd;

private:
	float CurrentStamina;
	float StaminaDefaultValue;
	float StaminaMaxValue;
	float ElapsedTime;
	float TimeSinceLastStaminaUse;
	float StaminaRecoveryTime;
	float StaminaLosingTime;
	bool bHasBall;
		
};
