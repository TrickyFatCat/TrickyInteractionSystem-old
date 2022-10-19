// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionLibrary.h"
#include "Engine/EngineTypes.h"
#include "InteractionQueueComponent.generated.h"

UCLASS(ClassGroup=(TrickyInteraction), meta=(BlueprintSpawnableComponent))
class TRICKYINTERACTIONSYSTEM_API UInteractionQueueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionQueueComponent();

protected:
	UPROPERTY(BlueprintReadWrite, Category="Interaction")
	bool bSortByWeight = true;

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

	UFUNCTION(BlueprintCallable, Category="TrickyInteractionSystem")
	bool Interact();

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

public:
	UFUNCTION(BlueprintGetter, Category="TrickyInteractionSystem")
	bool GetUseLineOfSight() const;

	UFUNCTION(BlueprintSetter, Category="TrickyInteractionSystem")
	void SetUseLineOfSight(const bool Value);
private:
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetUseLineOfSight, BlueprintSetter=SetUseLineOfSight, Category="Interaction", meta=(AllowPrivateAccess))
	bool bUseLineOfSight = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction", meta=(AllowPrivateAccess, EditCondition="bUseLineOfSight"))
	TEnumAsByte<ETraceTypeQuery> TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction", meta=(AllowPrivateAccess, EditCondition="bUseLineOfSight"))
	float SightDistance = 512.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction", meta=(AllowPrivateAccess, EditCondition="bUseLineOfSight"))
	float SightRadius = 32.f;

	UPROPERTY()
	AActor* ActorInSight = nullptr;

	AActor* GetActorInSight() const;

	void SortByLineOfSight(const AActor* Actor);
};
