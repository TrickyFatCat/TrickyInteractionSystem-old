// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "InteractionLibrary.h"
#include "CapsuleInteractionTrigger.generated.h"

/**
 * A trigger which adds and removes owner from the interaction queue.
 */
UCLASS(ClassGroup=(TrickyInteraction), meta=(BlueprintSpawnableComponent))
class TRICKYINTERACTIONSYSTEM_API UCapsuleInteractionTrigger : public UCapsuleComponent
{
	GENERATED_BODY()

public:
	UCapsuleInteractionTrigger();
	
	UFUNCTION(BlueprintSetter, Category="TrickyInteractionSystem")
	void SetInteractionSettings(const FInteractionData& Value);

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	void SetInteractionMessage(const FString& Message);
	
	UFUNCTION(BlueprintGetter, Category="TrickyInteractionSystem")
	FInteractionData GetInteractionSettings() const;
	
protected:
	virtual void BeginPlay() override;

private:
	/**
	 * Toggles if the trigger should behave as a normal trigger.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction", meta=(AllowPrivateAccess))
	bool bIsNormalTrigger = false;

	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetInteractionSettings,
		BlueprintSetter=SetInteractionSettings,
		Category="Interaction",
		meta=(AllowPrivateAccess))
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
