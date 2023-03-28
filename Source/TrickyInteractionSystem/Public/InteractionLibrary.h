// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InteractionLibrary.generated.h"

class UShapeComponent;

USTRUCT(BlueprintType)
struct FInteractionData
{
	GENERATED_BODY()

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

USTRUCT(BlueprintType)
struct FQueueData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QueueData")
	AActor* Actor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QueueData")
	FInteractionData InteractionData;
};

class UInteractionQueueComponent;

/**
 * A library which contains some useful functions for custom implementation of the system using Blueprints.
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQueueChangedSignature, const AActor*, OtherActor);

UCLASS()
class TRICKYINTERACTIONSYSTEM_API UInteractionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Returns player's viewport location and rotation. */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool GetPlayerViewpoint(const AActor* Actor, FVector& ViewLocation, FRotator& ViewRotation);

	/** Returns Interaction Queue Component if the given actor has one. */
	UFUNCTION(BlueprintPure, Category="TrickyInteractionSystem")
	static UInteractionQueueComponent* GetInteractionQueueComponent(const AActor* Actor);

	/** Checks if the interaction queue is empty. */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool IsInteractionQueueEmpty(const AActor* Actor);

	/** Adds actor to the interaction queue */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool AddToInteractionQueue(const AActor* Actor,
	                                  AActor* InteractiveActor,
	                                  const FInteractionData& InteractionData);

	/** Removes interaction data to the interaction queue component of the given actor if it was found. */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool RemoveFromInteractionQueue(const AActor* Actor, const AActor* InteractiveActor);

	/** Checks if the given actor has InteractionInterface.*/
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool HasInteractionInterface(const AActor* Actor);

	/** Check if the actor is in the interaction queue. */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool InteractionQueueHasActor(const AActor* Actor, const AActor* InteractiveActor);

	/** Returns interaction data of the given interactive actor. */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool GetInteractionData(const AActor* Actor,
	                               const AActor* InteractiveActor,
	                               FInteractionData& InteractionData);

	/** Returns first queue data. */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool GetFirstQueueData(const AActor* Actor, FQueueData& QueueData);

	/** Updates interaction message of the given actor in the interaction queue. */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool UpdateInteractionMessage(const AActor* Actor,
	                                     const AActor* InteractiveActor,
	                                     const FString& NewMessage);

	/** Updates interaction time of the give actor in the interaction queue. */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool UpdateInteractionTime(const AActor* Actor, const AActor* InteractiveActor, const float NewTime);

	/** Sets default collision for shape components which work as triggers. */
	static void SetTriggerDefaultCollision(UShapeComponent* ShapeComponent);
};
