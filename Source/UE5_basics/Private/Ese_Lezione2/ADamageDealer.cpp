// Fill out your copyright notice in the Description page of Project Settings.


#include "UE5_basics/Public/Ese_Lezione2/ADamageDealer.h"

#include "Components/BoxComponent.h"
#include "UE5_basics/Other/HealthComponent.h"

AADamageDealer::AADamageDealer()
{
	PrimaryActorTick.bCanEverTick = true; // can tick?
	PrimaryActorTick.TickInterval = 0.2f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	Amount = 1.0f;

}

void AADamageDealer::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AADamageDealer::OnBeginOverlap);
	
}

void AADamageDealer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AADamageDealer::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if(!IsValid(OtherActor))
	{
		return;
	}
	
	UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>();

	if (IsValid(HealthComponent))
	{
		HealthComponent->GetDamage(Amount);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "DAMAGE TAKEN: " + FString::SanitizeFloat(Amount));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "HEALTH REMAINING: " + FString::SanitizeFloat(HealthComponent->Health));
	}
}

