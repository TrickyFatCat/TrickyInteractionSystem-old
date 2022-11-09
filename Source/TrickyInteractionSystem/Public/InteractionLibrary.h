// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InteractionLibrary.generated.h"

class UInteractionQueueComponent;

USTRUCT(BlueprintType)
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	/**
	 * Toggles if the actor require being in the line of sight to be interacted. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionData")
	bool bRequireLineOfSight = false;

	/**
	 * A message which can be used in HUD.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionData")
	FString InteractionMessage = "Interact";

	/**
	 * A sort weight for sorting the interaction queue.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionData")
	int32 SortWeight = 0;

	/**
	 * How much time required to activate interaction effect.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionData", meta=(ClampMin="0"))
	float InteractionTime = 0.f;
};

/**
 * A library which contains some useful functions for custom implementation of the system using Blueprints.
 */
UCLASS()
class TRICKYINTERACTIONSYSTEM_API UInteractionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Returns player's viewport location and rotation.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool GetPlayerViewpoint(const AActor* Actor, FVector& ViewLocation, FRotator& ViewRotation);

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool AddToInteractionQueue(const AActor* Actor,
	                                  AActor* InteractiveActor,
	                                  const FInteractionData& InteractionData);

	/**
	 * Removes interaction data to the interaction queue component of the given actor if it was found.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool RemoveFromInteractionQueue(const AActor* Actor, const AActor* InteractiveActor);

	/**
	 * Checks if the given actor has InteractionInterface.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool HasInteractionInterface(const AActor* Actor);

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool IsActorInInteractionQueue(const AActor* Actor, const AActor* InteractiveActor);

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool GetInteractionData(const AActor* Actor,
	                               const AActor* InteractiveActor,
	                               FInteractionData& InteractionData);

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool UpdateInteractionMessage(const AActor* Actor, const AActor* InteractiveActor, const FString& NewMessage);
	
};
