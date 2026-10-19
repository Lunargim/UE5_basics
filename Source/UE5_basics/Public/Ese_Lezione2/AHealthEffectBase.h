
#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "AHealthEffectBase.generated.h"

UCLASS(Abstract)
class UE5_BASICS_API AHealthEffectBase : public AActor
{
	GENERATED_BODY()

public:
	AHealthEffectBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Effect")
	float Amount;
};