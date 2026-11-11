// Fill out your copyright notice in the Description page of Project Settings.


#include "Ese_Lezione5/StaminaComponent.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentStamina = 10.0f;
	StaminaMaxValue = 10.0f;
	ElapsedTime = 0.0f;
	TimeSinceLastStaminaUse = 0.0f;
	StaminaRecoveryTime = 2.0f;
	
	bHasBall = false;
	// ...
	
}


// Called every frame
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ChangeStamina(DeltaTime);
	EndStamina();
	// ...
}


void UStaminaComponent::SetHasBall(bool HasBall)
{
	bHasBall = HasBall;
	
}

void UStaminaComponent::ChangeStamina(float DeltaTime)
{
	if (!bHasBall) {
		ElapsedTime += DeltaTime;
		
		if (ElapsedTime >= StaminaRecoveryTime) {
			ElapsedTime = 0.0f;
			GainStamina(1);
			//OnStaminaRecovery.Broadcast();
		}		
	}else
	{
		ElapsedTime += DeltaTime;
		
		if (ElapsedTime >= StaminaRecoveryTime) {
			ElapsedTime = 0.0f;
			RemoveStamina(1);
			//OnStaminaRemoving.Broadcast();
		}		
	}
}

void UStaminaComponent::GainStamina(float Amount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + Amount, 0.0f, StaminaMaxValue);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
	FString::Printf(TEXT("Stamina: %.1f"), CurrentStamina));
}

void UStaminaComponent::RemoveStamina(float Amount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina - Amount, 0.0f, StaminaMaxValue);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
	FString::Printf(TEXT("Stamina: %.1f"), CurrentStamina));
}

void UStaminaComponent::EndStamina()
{
	if (bHasBall)
	{
		if (CurrentStamina <= 0)
		{
			OnStaminaEnd.Broadcast();
			
		}
	}
}

