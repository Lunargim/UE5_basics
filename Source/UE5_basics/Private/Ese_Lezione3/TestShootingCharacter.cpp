// Fill out your copyright notice in the Description page of Project Settings.

#include "Ese_Lezione3/TestShootingCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "Ese_Lezione4/Ball.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"

ATestShootingCharacter::ATestShootingCharacter()
{
 // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
 
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
 
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
 
	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
 
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false;

	BallSocket = CreateDefaultSubobject<USceneComponent>(TEXT("BallSocket"));
	BallSocket -> SetupAttachment(this->GetMesh());

	bHasBall=false;
	bCanShoot=true;
	Score=0;
}
 
// Called when the game starts or when spawned
void ATestShootingCharacter::BeginPlay()
{
	Super::BeginPlay();
 
	if(MovementCurve)
	{
		FOnTimelineFloat ProgressFunctionLength;
		ProgressFunctionLength.BindUFunction(this, "HandleProgressArmLength");
		AimTimeline.AddInterpFloat(MovementCurve, ProgressFunctionLength);
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Silver, "Bind the timeline!");
	}
}
 
// Registra l'Input Mapping Context quando il pawn viene posseduto
// (sostituisce i nodi "Add Input Mapping" del Blueprint)
void ATestShootingCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
 
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("DefaultMappingContext non assegnato nel BP!"));
			}
		}
	}
}
 
// Called every frame
void ATestShootingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
 
	AimTimeline.TickTimeline(DeltaTime);
}
 
/// Input function to override
/// @param PlayerInputComponent 
void ATestShootingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
 
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}
 
		// Moving
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATestShootingCharacter::Move);
		}
 
		// Looking (mouse e gamepad). Se MouseLookAction e LookAction sono lo stesso asset
		// si fa un solo bind, altrimenti il look viene applicato due volte.
		if (MouseLookAction && MouseLookAction != LookAction)
		{
			EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ATestShootingCharacter::Look);
		}
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATestShootingCharacter::Look);
		}
 
		// Aim
		//if (AimAction)
		//{
		//	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ATestShootingCharacter::AimIn);
		//	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ATestShootingCharacter::AimOut);
		//}
 
		// Shoot
		if (ShootAction)
		{
			EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ATestShootingCharacter::ShootLineTrace);
		}

		if (ShootBallAction)
		{
			EnhancedInputComponent->BindAction(ShootBallAction, ETriggerEvent::Started, this, &ATestShootingCharacter::ShootBall);
		}
	}
}
 
void ATestShootingCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
 
	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}
 
void ATestShootingCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
 
	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}
 
void ATestShootingCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
 
		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
 
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
 
		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}
 
void ATestShootingCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}
 
void ATestShootingCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}
 
void ATestShootingCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void ATestShootingCharacter::IncreaseScore()
{
	Score++;

	GEngine->AddOnScreenDebugMessage(
	static_cast<uint64>(GetUniqueID()), 5.f, FColor::Green,
	FString::Printf(TEXT("%s -> Punti: %d"), *GetName(), Score));
}

void ATestShootingCharacter::BlockInput()
{
	bHasBall = true;
}

void ATestShootingCharacter::AimIn()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	AimTimeline.Play();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, "Aim IN");
}
 
void ATestShootingCharacter::AimOut()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	AimTimeline.Reverse();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "Aim OUT");
}
 
void ATestShootingCharacter::HandleProgressArmLength(float Length)
{
	CameraBoom->TargetArmLength = Length;
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("New camer boom val -> %f"), Length));
}
 
void ATestShootingCharacter::HandleProgressCameraOffset(FVector Offset) {
	CameraBoom->SocketOffset = Offset;
}
 
void ATestShootingCharacter::ShootLineTrace()
{
	if (GetWorldTimerManager().IsTimerActive(CooldownTimer)) return;
	if (bHasBall) return;
	
	AController* PC = GetController();
	if (!PC) return;
	
	FVector Start;
	FRotator Rot;
	PC->GetPlayerViewPoint(Start, Rot);

	Start += Rot.Vector() * CameraBoom->TargetArmLength; //starting from character body
	const float Range = 500.f;
	const FVector End = Start + Rot.Vector() * Range;
 
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
 
	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit, Start, End, ECC_Visibility, Params);
	
	DrawDebugLine(GetWorld(), Start, bHit ? Hit.ImpactPoint : End,
		bHit ? FColor::Green : FColor::Red, false, 2.f, 0, 1.f);
 
	if (bHit)
	{
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.f, 12, FColor::Yellow, false, 2.f);
 
		if (AActor* HitActor = Hit.GetActor())
		{
			if (ABall* HitBall = Cast<ABall>(HitActor))
			{
				CurrentBall = HitBall;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan,
				FString::Printf(TEXT("Colpito: %s"), *HitActor->GetName()));
				CurrentBall->BroadcastBallActions(this);
			}
		}
	}

	GetWorldTimerManager().SetTimer(CooldownTimer, 1.f, false);
}

void ATestShootingCharacter::ShootBall()
{
	if (!bHasBall) return;
	if (!IsValid(CurrentBall)) return;
	
	CurrentBall->ShotBall(FollowCamera->GetForwardVector(), 1000.f);
	
	ClearBall();
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan,
				FString::Printf(TEXT("PallaSparata")));
	
}

void ATestShootingCharacter::ClearBall()
{
	bHasBall = false;
	CurrentBall = nullptr;
}
