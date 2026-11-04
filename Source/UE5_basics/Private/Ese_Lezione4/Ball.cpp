// Fill out your copyright notice in the Description page of Project Settings.


#include "Ese_Lezione4/Ball.h"

#include "Ese_Lezione3/TestShootingCharacter.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BallSphere = CreateDefaultSubobject<USphereComponent>(TEXT("BallSphere"));
	RootComponent = BallSphere;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(RootComponent);

	BallSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BallSphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	BallSphere->SetSimulatePhysics(true);

	BallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();

	OnBallTaken.AddDynamic(this, &ABall::MoveForward);
	OnBallTaken.AddDynamic(this, &ABall::SetCurrentPlayer);
	OnBallTaken.AddDynamic(this, &ABall::StartScoreTimer);
	
}

void ABall::BroadcastBallActions(ATestShootingCharacter* Player)
{
	if (Player == CurrentPlayer) return;
	BallSphere->SetSimulatePhysics(false);
	BallSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OnBallTaken.Broadcast(Player);
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABall::MoveForward(ATestShootingCharacter* Player)
{
	if (!Player) return;
	
	AttachToComponent(Player->BallSocket,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ABall::SetCurrentPlayer(ATestShootingCharacter* Player)
{
	if (!Player) return;

	if (CurrentPlayer && CurrentPlayer != Player)
	{
		CurrentPlayer->ClearBall();
	}
	
	CurrentPlayer = Player;
	CurrentPlayer -> BlockInput();
}

void ABall::StartScoreTimer(ATestShootingCharacter* Player)
{
	if (!Player) return;

	GetWorldTimerManager().SetTimer(
	ScoreTimer, this, &ABall::AddScoring, 5.f, true);
}

void ABall::AddScoring()
{
	if (!CurrentPlayer) return;

	CurrentPlayer->IncreaseScore();
}

void ABall::ShotBall(const FVector& Direction, float Strength)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	BallSphere->SetCollisionProfileName(TEXT("PhysicsActor"));
	BallSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BallSphere->SetSimulatePhysics(true);

	BallSphere->SetCollisionProfileName(TEXT("PhysicsActor"));
	BallSphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	BallSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	BallSphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	BallSphere->AddImpulse(Direction * Strength, NAME_None, true);

	CurrentPlayer = nullptr;
}





