// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionLibrary.h"
#include "Engine/EngineTypes.h"
#include "InteractionQueueComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionStartedSignature, AActor*, TargetActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractSignature, AActor*, TargetActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionStoppedSignature, AActor*, TargetActor);

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

	/**
	 * Called when the interaction process started.
	 */
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteractionStartedSignature OnInteractionStarted;

	/**
	 * Called when the interaction effect successfully activated.
	 */
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteractSignature OnInteract;

	/**
	 * Called when the interaction process stopped.
	 */
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteractionStoppedSignature OnInteractionStopped;

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	bool Add(const FInteractionData& InteractionData);

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	bool Remove(const FInteractionData& InteractionData);

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	bool RemoveActor(const AActor* Actor);

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	bool StartInteraction();

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	bool StopInteraction();

	UFUNCTION(BlueprintPure, Category="TrickyInteractionSystem")
	bool IsQueueEmpty() const;

	UFUNCTION(BlueprintPure, Category="TrickyInteractionSystem")
	bool QueueHasData(const FInteractionData& InteractionData) const;

	UFUNCTION(BlueprintPure, Category="TrickyInteractionSystem")
	bool QueueHasActor(const AActor* Actor) const;

	UFUNCTION(BlueprintPure, Category="TrickyInteractionSystem")
	void GetFirstDataInQueue(UPARAM(ref) FInteractionData& Data);

	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction", meta=(AllowPrivateAccess))
	TArray<FInteractionData> InteractionQueue;

	void SortByWeight();

	UFUNCTION()
	bool Interact(const FInteractionData& InteractionData) const;

// Line of sight logic
	
public:
	UFUNCTION(BlueprintGetter, Category="TrickyInteractionSystem")
	bool GetUseLineOfSight() const;

	UFUNCTION(BlueprintSetter, Category="TrickyInteractionSystem")
	void SetUseLineOfSight(const bool Value);
private:
	/**
	 * Toggles the line of sight checks.
	 *
	 * Keep it false if there's no interactive actors require line of sight to interact with.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetUseLineOfSight, BlueprintSetter=SetUseLineOfSight, Category="Interaction", meta=(AllowPrivateAccess))
	bool bUseLineOfSight = false;

	/**
	 * Line of sight trace channel.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction", meta=(AllowPrivateAccess, EditCondition="bUseLineOfSight"))
	TEnumAsByte<ETraceTypeQuery> TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);

	/**
	 * The line of sight max distance.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction", meta=(AllowPrivateAccess, EditCondition="bUseLineOfSight"))
	float SightDistance = 512.f;

	/**
	 * The line of sight radius. 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction", meta=(AllowPrivateAccess, EditCondition="bUseLineOfSight"))
	float SightRadius = 32.f;

	/**
	 * The actor caught by line of sight.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction", meta=(AllowPrivateAccess))
	AActor* ActorInSight = nullptr;

	AActor* GetActorInSight() const;

	void SortByLineOfSight(const AActor* Actor);

// Overtime interaction
private:
	UPROPERTY(BlueprintReadOnly, Category="Interaction", meta=(AllowPrivateAccess))
	FTimerHandle InteractionTimer;

	bool StartInteractionTimer(const FInteractionData& InteractionData);

	UFUNCTION()
	void InteractWrapper(const FInteractionData& InteractionData) const;

	bool IsInteractionTimerActive() const;
};
