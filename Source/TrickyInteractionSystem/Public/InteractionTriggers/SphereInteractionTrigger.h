// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "InteractionLibrary.h"
#include "Components/SphereComponent.h"
#include "SphereInteractionTrigger.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(TrickyInteraction), meta=(BlueprintSpawnableComponent))
class TRICKYINTERACTIONSYSTEM_API USphereInteractionTrigger : public USphereComponent
{
	GENERATED_BODY()

public:
	USphereInteractionTrigger();

	UFUNCTION(BlueprintSetter, Category="TrickyInteractionSystem")
	void SetInteractionSettings(const FInteractionData& Value);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction", meta=(AllowPrivateAccess))
	bool bIsNormalTrigger = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintSetter=SetInteractionSettings, Category="Interaction", meta=(AllowPrivateAccess))
	FInteractionData InteractionSettings;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                            AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex,
	                            bool bFromSweep,
	                            const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
	                          AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp,
	                          int32 OtherBodyIndex);
};
