// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "TestShootingCharacter.generated.h"

class UInputMappingContext;

UCLASS()
class UE5_BASICS_API ATestShootingCharacter : public ACharacter
{
    GENERATED_BODY()
 
public:
    
    ATestShootingCharacter();
    
    UPROPERTY(EditAnywhere, Category = "Timeline")
    UCurveFloat* MovementCurve;
 
    UPROPERTY(EditAnywhere, Category = "Timeline")
    UCurveVector* OffsetCurve;
 
    /** Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;
 
    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;
 
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
 
    // Chiamata quando il pawn viene posseduto: qui registriamo l'Input Mapping Context
    virtual void PawnClientRestart() override;
 
    FTimeline AimTimeline;
 
    /** Called for movement input */
    void Move(const FInputActionValue& Value);
 
    /** Called for looking input */
    void Look(const FInputActionValue& Value);
 
    /** Input Mapping Context da registrare (es. IMC_Default) */
    UPROPERTY(EditAnywhere, Category="Input")
    UInputMappingContext* DefaultMappingContext;
    
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* JumpAction;
 
    /** Move Input Action */
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* MoveAction;
 
    /** Look Input Action */
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* LookAction;
 
    /** Mouse Look Input Action */
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* MouseLookAction;
    
    /** Aim Input Action */
    //UPROPERTY(EditAnywhere, Category="Input")
    //UInputAction* AimAction;
 
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* ShootAction;
 
public: 
    // Called every frame
    virtual void Tick(float DeltaTime) override;
 
    void AimIn();
    void AimOut();
    
    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
 
    /** Handles move inputs from either controls or UI interfaces */
    UFUNCTION(BlueprintCallable, Category="Input")
    virtual void DoMove(float Right, float Forward);
 
    /** Handles look inputs from either controls or UI interfaces */
    UFUNCTION(BlueprintCallable, Category="Input")
    virtual void DoLook(float Yaw, float Pitch);
 
    /** Handles jump pressed inputs from either controls or UI interfaces */
    UFUNCTION(BlueprintCallable, Category="Input")
    virtual void DoJumpStart();
 
    /** Handles jump released inputs from either controls or UI interfaces */
    UFUNCTION(BlueprintCallable, Category="Input")
    virtual void DoJumpEnd();
 
private:
    UFUNCTION()
    void HandleProgressArmLength(float Length);
    
    UFUNCTION()
    void HandleProgressCameraOffset(FVector Offset);
 
    UFUNCTION(BlueprintCallable)
    void ShootLineTrace();
};
