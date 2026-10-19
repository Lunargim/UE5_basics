// Fill out your copyright notice in the Description page of Project Settings.

#include "UE5_basics/Public/Ese_Lezione2/AHealthGiver.h"
#include "UE5_basics/Other/HealthComponent.h"
#include "UE5_basics/Public/Ese_Lezione2/ADamageDealer.h"


AAHealthGiver::AAHealthGiver()
{
	PrimaryActorTick.bCanEverTick = true; // can tick?
	PrimaryActorTick.TickInterval = 0.2f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	Amount = 2.0f;

}

void AAHealthGiver::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AAHealthGiver::OnBeginOverlap);
	
}

void AAHealthGiver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAHealthGiver::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if(!IsValid(OtherActor))
	{
		return;
	}
	
	UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>();

	if (IsValid(HealthComponent))
	{
		HealthComponent->Healing(Amount);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "HEALTH TAKEN: " + FString::SanitizeFloat(Amount));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "HEALTH REMAINING: " + FString::SanitizeFloat(HealthComponent->Health));
	}
}

