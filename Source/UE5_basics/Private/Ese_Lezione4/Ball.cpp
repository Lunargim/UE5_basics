// Fill out your copyright notice in the Description page of Project Settings.


#include "Ese_Lezione4/Ball.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BallSphere = CreateDefaultSubobject<USphereComponent>(TEXT("BallSphere"));
	RootComponent = BallSphere;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(RootComponent);

	BallSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BallSphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	BallSphere->SetSimulatePhysics(false);

	BallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABall::MoveForward(USceneComponent* PlayerSocket)
{
	if (!PlayerSocket) return;
	
	AttachToComponent(PlayerSocket,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}



