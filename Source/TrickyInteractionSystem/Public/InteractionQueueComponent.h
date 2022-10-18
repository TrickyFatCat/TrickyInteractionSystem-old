// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionQueueComponent.generated.h"

USTRUCT(BlueprintType)
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	/**
	 * An actor which has an interaction interface.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="InteractionData")
	AActor* Actor = nullptr;

	/**
	 * Determines if the actor required to be in line of sight for interaction.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="InteractionData")
	bool bRequireLineOfSight = false;

	/**
	 * A message which can be used in HUD to show a type of interaction.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="InteractionData")
	FString InteractionMessage = "Interact";

	static bool Equal(const FInteractionData& Data_A, const FInteractionData& Data_B)
	{
		return Data_A.Actor == Data_B.Actor
				&& Data_A.bRequireLineOfSight == Data_B.bRequireLineOfSight
				&& Data_A.InteractionMessage == Data_B.InteractionMessage;
	}
};

UCLASS(ClassGroup=(TrickyInteraction), meta=(BlueprintSpawnableComponent))
class TRICKYINTERACTIONSYSTEM_API UInteractionQueueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionQueueComponent();

public:
	virtual void TickComponent(float DeltaTime,
	                           ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	bool Add(const FInteractionData& InteractionData);

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	bool Remove(const FInteractionData& InteractionData);

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	bool RemoveActor(const AActor* Actor);

	UFUNCTION(BlueprintPure, Category="TrickyInteractionSystem")
	bool QueueHasData(const FInteractionData& InteractionData) const;

	UFUNCTION(BlueprintPure, Category="TrickyInteractionSystem")
	bool QueueHasActor(const AActor* Actor) const;
private:
	UPROPERTY(VisibleAnywhere, Category="Interaction")
	TArray<FInteractionData> InteractionQueue;
};
