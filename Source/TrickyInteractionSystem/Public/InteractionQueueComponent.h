// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionLibrary.h"
#include "Engine/EngineTypes.h"
#include "InteractionQueueComponent.generated.h"

DECLARE_LOG_CATEGORY_CLASS(LogInteractionQueueComponent, Display, Display)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionSignature, AActor*, TargetActor);

/**
 *  This component handles creating a queue for interaction which it sorts by weight and line of sight.
 */
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

	/** Called when the interaction process started. */
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteractionSignature OnInteractionStarted;

	/** Called when the interaction effect successfully activated. */
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteractionSignature OnInteractionFinishedSignature;

	/** Called when the interaction process stopped. */
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteractionSignature OnInteractionStopped;

	/** Called when actor was added to the queue. */
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnQueueChangedSignature OnActorAdded;

	/** Called when actor was added to the queue. */
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnQueueChangedSignature OnActorRemoved;

	/** Adds interaction data into the interaction queue. */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	bool Add(AActor* Actor, const FInteractionData& InteractionData);

	/** Removes interaction data into the interaction queue. */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	bool Remove(const AActor* Actor);

	/** Starts interaction with the first actor in the interaction queue. */
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	bool StartInteraction();

	/** Finishes interaction.
	 * Call it if FinishManually == false.
	 */
	UFUNCTION()
	bool FinishInteraction(AActor* Actor);

	/** Stops interaction.*/
	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	bool StopInteraction();

	/** Checks if the interaction queue is empty.*/
	UFUNCTION(BlueprintPure, Category="TrickyInteractionSystem")
	bool IsQueueEmpty() const;

	/** Checks if the interaction queue has interaction data with the given actor.*/
	UFUNCTION(BlueprintPure, Category="TrickyInteractionSystem")
	bool QueueHasActor(const AActor* Actor) const;

	/** Returns the first interaction data in queue.*/
	UFUNCTION(BlueprintPure, Category="TrickyInteractionSystem")
	void GetFirstInteractionData(FInteractionData& Data);

	/** Returns the first actor in the queue.*/
	UFUNCTION(BlueprintPure, Category="TrickyInteractionSystem")
	AActor* GetFirstActor();

	/** Returns the firs actor and its interaction data.*/
	UFUNCTION(BlueprintPure, Category="TrickyInteractionSystem")
	bool GetFirstQueueData(FQueueData& QueueData);

	/** Returns interaction data of the given actor.*/
	UFUNCTION(BlueprintPure, Category="TrickyInteractionSystem")
	bool GetInteractionData(const AActor* Actor, FInteractionData& Data);

	/** Updates interaction message of the given actor.*/
	UFUNCTION(BlueprintPure, Category="TrickyInteractionSystem")
	bool UpdateInteractionMessage(const AActor* Actor, const FString& NewMessage);

	/** Updates interaction time of the given actor.*/
	UFUNCTION(BlueprintPure, Category="TrickyInteractionSystem")
	bool UpdateInteractionTime(const AActor* Actor, float NewTime);

private:
	/** If true, the FinishInteraction() must be called manually.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction", meta=(AllowPrivateAccess))
	bool bFinishManually = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction", meta=(AllowPrivateAccess))
	TArray<FQueueData> InteractionQueue;

	void SortByWeight();

	void LogWarning(const FString& Message) const;

	FInteractionData& FindInteractionData(const AActor* Actor);

	// Line of sight logic

public:
	UFUNCTION(BlueprintGetter, Category="TrickyInteractionSystem")
	bool GetUseLineOfSight() const;

	UFUNCTION(BlueprintSetter, Category="TrickyInteractionSystem")
	void SetUseLineOfSight(const bool Value);

private:
	/**
	 * Toggles the line of sight checks.
	 * Keep it false if there's no interactive actors require line of sight to interact with.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetUseLineOfSight, BlueprintSetter=SetUseLineOfSight,
		Category="Interaction", meta=(AllowPrivateAccess))
	bool bUseLineOfSight = false;

	/** Line of sight trace channel. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction",
		meta=(AllowPrivateAccess, EditCondition="bUseLineOfSight"))
	TEnumAsByte<ETraceTypeQuery> TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);

	/** The line of sight max distance.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction",
		meta=(AllowPrivateAccess, EditCondition="bUseLineOfSight"))
	float SightDistance = 512.f;

	/** The line of sight radius. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction",
		meta=(AllowPrivateAccess, EditCondition="bUseLineOfSight"))
	float SightRadius = 32.f;

	/** The actor caught by line of sight. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction", meta=(AllowPrivateAccess))
	AActor* ActorInSight = nullptr;

	AActor* GetActorInSight() const;

	void SortByLineOfSight(const AActor* Actor);

	// Overtime interaction
	UPROPERTY(BlueprintReadOnly, Category="Interaction", meta=(AllowPrivateAccess))
	FTimerHandle InteractionTimer;

	bool StartInteractionTimer(const FQueueData& QueueData);

	UFUNCTION()
	void FinishInteractionWrapper(AActor* Actor);

	bool IsInteractionTimerActive() const;
};
