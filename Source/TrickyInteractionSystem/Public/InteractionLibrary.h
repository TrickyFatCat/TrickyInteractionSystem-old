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
	 * An interactive actor which InteractionInterface implemented.
	 */
	UPROPERTY(VisibleAnywhere, Category="InteractionData")
	AActor* Actor = nullptr;

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

	/**
	 * If true, the system will call the Interact function from InteractionInterface.
	 * Make it false if you want to make interaction depending on holding button.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionData")
	bool bCallInteractFunction = true;

	static bool Equal(const FInteractionData& Data_A, const FInteractionData& Data_B)
	{
		return Data_A.Actor == Data_B.Actor
				&& Data_A.bRequireLineOfSight == Data_B.bRequireLineOfSight
				&& Data_A.InteractionMessage == Data_B.InteractionMessage
				&& Data_A.SortWeight == Data_B.SortWeight
				&& Data_A.InteractionTime == Data_B.InteractionTime;
	}

	static void SetActor(AActor* Actor, FInteractionData& Data)
	{
		Data.Actor = Actor;
	}
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
	 * Compare interaction data and returns if they're equal or not.
	 */
	UFUNCTION(BlueprintPure, Category="TrickyInteractionSystem")
	static bool Equal(const FInteractionData& Data_A, const FInteractionData& Data_B)
	{
		return FInteractionData::Equal(Data_A, Data_B);
	}
	
	/**
	 * Sets the Actor variable in a given interaction data.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static void SetActor(AActor* Actor, UPARAM(ref) FInteractionData& InteractionData)
	{
		FInteractionData::SetActor(Actor, InteractionData);
	}
	
	/**
	 * Returns player's viewport location and rotation.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool GetPlayerViewpoint(const AActor* Actor, FVector& ViewLocation, FRotator& ViewRotation);

	static bool AddDataToQueue(const AActor* Actor, const FInteractionData& InteractionData);

	static bool RemoveDataFromQueue(const AActor* Actor, const FInteractionData& InteractionData);
	
	/**
	 * Adds custom interaction data to the interaction queue component of the given actor.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool AddToQueue(const AActor* TargetActor,
	                       AActor* InteractiveActor,
	                       const bool bRequireLineOfSight = false,
	                       const FString& InteractionMessage = "Interact",
	                       const int32 SortWeight = 0,
	                       float InteractionTime = 0.f);
	
	/**
	 * Removes interaction data to the interaction queue component of the given actor if it was found.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool RemoveFromQueue(const AActor* TargetActor, const AActor* InteractiveActor);
	
	/**
	 * Checks if the given actor has InteractionInterface.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool HasInteractionInterface(const AActor* Actor);
};
