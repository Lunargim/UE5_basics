// Fill out your copyright notice in the Description page of Project Settings.


#include "Ese_Lezione3/TargetBox.h"

#include "Components/BoxComponent.h"

// Sets default values
ATargetBox::ATargetBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

	BoxComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATargetBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATargetBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATargetBox::GetHit()
{
	Score =+ 1.0f;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Colpito, Score: "+ FString::SanitizeFloat(Score));
}

