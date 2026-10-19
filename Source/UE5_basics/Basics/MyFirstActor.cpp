// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFirstActor.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AMyFirstActor::AMyFirstActor()
{
    PrimaryActorTick.bCanEverTick = true; // can tick?
    PrimaryActorTick.TickInterval = 0.2f; // what is the interval tick?

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    /* 
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    BoxComponent->SetGenerateOverlapEvents(true);
    BoxComponent->SetBoxExtent(FVector(200.f, 200.f, 100.f));
    BoxComponent->SetCanEverAffectNavigation(false);
    */
    BoxComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMyFirstActor::BeginPlay()
{
    Super::BeginPlay();
    
    OnActorBeginOverlap.AddDynamic(this, &AMyFirstActor::OnBeginOverlap);
    OnActorEndOverlap.AddDynamic(this, &AMyFirstActor::OnEndOverlap);

	//for showing when character starting inside
    //GetWorldTimerManager().SetTimerForNextTick(this, &AMyFirstActor::SyncInitialActors);

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if(IsValid(PC))
    {
       EnableInput(PC);
       if(InputComponent)
       {
          InputComponent->BindKey(EKeys::K, IE_Pressed, this, &AMyFirstActor::DebugPrintActors);
       }
    }

    // Bind example
    /*
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AFP_FirstPersonCharacter * FPCharacter = dynamic_cast<AFP_FirstPersonCharacter*>(PlayerCharacter);
    if(IsValid(FPCharacter))
    {
        FPCharacter->OnLineTraceCast.AddDynamic(this, &AMyFirstActor::BindTest);
    }
    */
}

void AMyFirstActor::SyncInitialActors()
{
    for(auto& SensedClass : SensedCharacterClasses)
    {
       TArray<AActor*> Found;
       GetOverlappingActors(Found, SensedClass);
       for(AActor* Actor : Found)
       {
          ActorsInside.AddUnique(Actor);
          Actor->OnDestroyed.AddUniqueDynamic(this, &AMyFirstActor::OnInsideActorDestroyed);
       }
    }

    DebugPrintActors();
}

void AMyFirstActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    DebugPrintActors();

    Super::EndPlay(EndPlayReason);
}

void AMyFirstActor::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if(!IsValid(OtherActor))
    {
       return;
    }

    UClass* OtherActorClass = OtherActor->GetClass();
    for(auto& SensedClass : SensedCharacterClasses)
    {
       if(SensedClass == OtherActorClass || OtherActorClass->IsChildOf(SensedClass))
       {
          ActorsInside.AddUnique(OtherActor);
          OtherActor->OnDestroyed.AddUniqueDynamic(this, &AMyFirstActor::OnInsideActorDestroyed);
          GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "IN: "+OtherActor->GetName());
          //OnSomethingEnter();
          break;
       }
    }
    
    /*
    UWorld* World = GetWorld();
    if(!IsValid(World))
    {
       return;
    }
    
    if(OtherActor == UGameplayStatics::GetPlayerPawn(World, 0))
    {
       GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "IN: "+OtherActor->GetName());
    }
    */
}

void AMyFirstActor::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if(!IsValid(OtherActor))
    {
       return;
    }

    UClass* OtherActorClass = OtherActor->GetClass();
    for(auto& SensedClass : SensedCharacterClasses)
    {
       if(SensedClass == OtherActorClass || OtherActorClass->IsChildOf(SensedClass))
       {
          ActorsInside.Remove(OtherActor);
          OtherActor->OnDestroyed.RemoveDynamic(this, &AMyFirstActor::OnInsideActorDestroyed);
          GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "OUT: "+OtherActor->GetName());
       }
    }

    /*
    UWorld* World = GetWorld();
    if(!IsValid(World))
    {
       return;
    }
    
    if(OtherActor == UGameplayStatics::GetPlayerPawn(World, 0))
    {
       GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "OUT: "+OtherActor->GetName());
    }
    */
}

void AMyFirstActor::OnInsideActorDestroyed(AActor* DestroyedActor)
{
    ActorsInside.Remove(DestroyedActor);
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "DESTROYED: "+DestroyedActor->GetName());
}

void AMyFirstActor::DebugPrintActors()
{
    GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Green, "Actors inside: "+FString::FromInt(ActorsInside.Num()));

    for(AActor* Actor : ActorsInside)
    {
       if(IsValid(Actor))
       {
          GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Green, "- "+Actor->GetName());
       }
    }
}

void AMyFirstActor::BindTest()
{
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple, "Player has shoot!");
}

// Called every frame
void AMyFirstActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //GEngine->AddOnScreenDebugMessage(896745, 2.f, FColor::Green, FString::Printf(TEXT("world time: %f"), GetWorld()->TimeSeconds));
    //GEngine->AddOnScreenDebugMessage(8967444, 2.f, FColor::Blue, "world time: "+FString::SanitizeFloat(GetWorld()->TimeSeconds));
}