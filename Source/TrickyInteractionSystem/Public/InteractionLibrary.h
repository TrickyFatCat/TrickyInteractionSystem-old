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
	 * An actor which has an interaction interface.
	 */
	UPROPERTY(VisibleAnywhere, Category="InteractionData")
	AActor* Actor = nullptr;

	/**
	 * Determines if the actor required to be in line of sight for interaction.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionData")
	bool bRequireLineOfSight = false;

	/**
	 * A message which can be used in HUD to show a type of interaction.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionData")
	FString InteractionMessage = "Interact";

	/**
	 * A sort weight for sorting in queue
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionData")
	int32 SortWeight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionData", meta=(ClampMin="0"))
	float InteractionTime = 0.f;

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
 * 
 */
UCLASS()
class TRICKYINTERACTIONSYSTEM_API UInteractionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="TrickyInteractionSystem")
	static bool Equal(const FInteractionData& Data_A, const FInteractionData& Data_B)
	{
		return FInteractionData::Equal(Data_A, Data_B);
	}

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static void SetActor(AActor* Actor, UPARAM(ref) FInteractionData& InteractionData)
	{
		FInteractionData::SetActor(Actor, InteractionData);
	}

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool GetPlayerViewpoint(const AActor* Actor, FVector& ViewLocation, FRotator& ViewRotation);

	static bool AddDataToQueue(const AActor* Actor, const FInteractionData& InteractionData);

	static bool RemoveDataFromQueue(const AActor* Actor, const FInteractionData& InteractionData);

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool AddToQueue(const AActor* TargetActor,
	                       AActor* InteractiveActor,
	                       const bool bRequireLineOfSight = false,
	                       const FString& InteractionMessage = "Interact",
	                       const int32 SortWeight = 0,
	                       float InteractionTime = 0.f);

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool RemoveFromQueue(const AActor* TargetActor, const AActor* InteractiveActor);

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	static bool HasInteractionInterface(const AActor* Actor);
};
