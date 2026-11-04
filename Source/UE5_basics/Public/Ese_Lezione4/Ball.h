// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

class ATestShootingCharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBallTaken, ATestShootingCharacter*, Player);

UCLASS()
class UE5_BASICS_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();

	UPROPERTY(EditAnywhere)
	USphereComponent* BallSphere;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BallMesh;

	UPROPERTY()
	FOnBallTaken OnBallTaken;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	ATestShootingCharacter *CurrentPlayer;

	UFUNCTION()
	void MoveForward(ATestShootingCharacter* Player);
	UFUNCTION()
	void SetCurrentPlayer(ATestShootingCharacter* Player);
	UFUNCTION()
	void StartScoreTimer(ATestShootingCharacter* Player);
	void AddScoring();

	FTimerHandle ScoreTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BroadcastBallActions(ATestShootingCharacter* Player);

	void ShotBall(const FVector& Direction, float Strength);
};
