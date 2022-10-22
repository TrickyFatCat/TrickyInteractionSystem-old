// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov


#include "InteractionQueueComponent.h"

#include "InteractionLibrary.h"
#include "InteractionInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

UInteractionQueueComponent::UInteractionQueueComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickInterval(0.05f);
}

void UInteractionQueueComponent::TickComponent(float DeltaTime,
                                               ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsQueueEmpty())
	{
		ActorInSight = GetActorInSight();
		SortByLineOfSight(ActorInSight);
	}
}

bool UInteractionQueueComponent::Add(const FInteractionData& InteractionData)
{
	if (!IsValid(InteractionData.Actor) && QueueHasData(InteractionData))
	{
		return false;
	}

	InteractionQueue.Add(InteractionData);
	SortByWeight();
	return true;
}

bool UInteractionQueueComponent::Remove(const FInteractionData& InteractionData)
{
	if (!QueueHasData(InteractionData))
	{
		return false;
	}

	auto Predicate = [&](const FInteractionData& Data) { return FInteractionData::Equal(Data, InteractionData); };
	const bool bItemsRemoved = InteractionQueue.RemoveAll(Predicate) > 0;

	if (bItemsRemoved)
	{
		SortByWeight();
	}

	return bItemsRemoved;
}

bool UInteractionQueueComponent::RemoveActor(const AActor* Actor)
{
	if (!IsValid(Actor) || !QueueHasActor(Actor))
	{
		return false;
	}

	auto Predicate = [&](const FInteractionData& Data) { return Data.Actor == Actor; };
	const bool bItemsRemoved = InteractionQueue.RemoveAll(Predicate) > 0;

	if (bItemsRemoved)
	{
		SortByWeight();
	}

	return bItemsRemoved;
}

bool UInteractionQueueComponent::StartInteraction()
{
	if (IsQueueEmpty())
	{
		return false;
	}

	FInteractionData InteractionData;
	GetFirstDataInQueue(InteractionData);

	if (!IsValid(InteractionData.Actor))
	{
		return false;
	}

	if (! InteractionData.Actor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		return false;
	}

	if (InteractionData.bRequireLineOfSight && InteractionData.Actor != ActorInSight)
	{
		return false;
	}

	if (InteractionData.InteractionTime > 0.f)
	{
		return StartInteractionTimer(InteractionData);
	}

	if (Interact(InteractionData))
	{
		OnInteractionStarted.Broadcast();
		return true;
	}

	return false;
}

bool UInteractionQueueComponent::StopInteraction()
{
	if (!GetWorld())
	{
		return false;
	}

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (!TimerManager.IsTimerActive(InteractionTimer))
	{
		return false;
	}

	FInteractionData InteractionData;
	GetFirstDataInQueue(InteractionData);
	IInteractionInterface::Execute_StopInteraction(InteractionData.Actor, GetOwner());
	TimerManager.ClearTimer(InteractionTimer);
	OnInteractionStopped.Broadcast();
	return true;
}

bool UInteractionQueueComponent::IsQueueEmpty() const
{
	return InteractionQueue.Num() == 0;
}

bool UInteractionQueueComponent::QueueHasData(const FInteractionData& InteractionData) const
{
	auto Predicate = [&](const FInteractionData& Data)
	{
		return FInteractionData::Equal(Data, InteractionData);
	};

	return InteractionQueue.ContainsByPredicate(Predicate);
}

bool UInteractionQueueComponent::QueueHasActor(const AActor* Actor) const
{
	auto Predicate = [&](const FInteractionData& Data) { return Data.Actor == Actor; };
	return InteractionQueue.ContainsByPredicate(Predicate);
}

void UInteractionQueueComponent::GetFirstDataInQueue(FInteractionData& Data)
{
	if (IsQueueEmpty())
	{
		return;
	}

	Data = InteractionQueue[0];
}

void UInteractionQueueComponent::SortByWeight()
{
	if (InteractionQueue.Num() <= 1)
	{
		return;
	}

	auto PredicateWeight = [&](const FInteractionData& Data_A, const FInteractionData& Data_B)
	{
		return Data_A.SortWeight >= Data_B.SortWeight && Data_A.bRequireLineOfSight <= Data_B.bRequireLineOfSight;
	};

	InteractionQueue.Sort(PredicateWeight);
}

bool UInteractionQueueComponent::Interact(const FInteractionData& InteractionData) const
{
	if (!IsValid(InteractionData.Actor))
	{
		return false;
	}

	if (InteractionData.bRequireLineOfSight && InteractionData.Actor != ActorInSight)
	{
		return false;
	}

	const bool bResult = IInteractionInterface::Execute_Interact(InteractionData.Actor, GetOwner());
	
	if (bResult)
	{
		OnInteractionFinished.Broadcast();
		return true;
	}

	return false;
}

bool UInteractionQueueComponent::GetUseLineOfSight() const
{
	return bUseLineOfSight;
}

void UInteractionQueueComponent::SetUseLineOfSight(const bool Value)
{
	bUseLineOfSight = Value;
	SetComponentTickEnabled(bUseLineOfSight);
	SortByWeight();
}

AActor* UInteractionQueueComponent::GetActorInSight() const
{
	FVector ViewLocation{FVector::ZeroVector};
	FRotator ViewRotation{FRotator::ZeroRotator};

	if (!UInteractionLibrary::GetPlayerViewpoint(GetOwner(), ViewLocation, ViewRotation))
	{
		return nullptr;
	}

	const FVector TraceStart{ViewLocation};
	const FVector TraceDirection{ViewRotation.Vector()};
	const FVector TraceEnd{TraceStart + TraceDirection * SightDistance};

	if (!GetWorld())
	{
		return nullptr;
	}

	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
	                                        TraceStart,
	                                        TraceEnd,
	                                        SightRadius,
	                                        TraceChannel,
	                                        false,
	                                        {GetOwner()},
	                                        EDrawDebugTrace::None,
	                                        HitResult,
	                                        true,
	                                        FLinearColor::Red,
	                                        FLinearColor::Green,
	                                        .05f);

	return HitResult.GetActor();
}

void UInteractionQueueComponent::SortByLineOfSight(const AActor* Actor)
{
	FInteractionData InteractionData;

	if (!IsValid(Actor) || !QueueHasActor(Actor))
	{
		GetFirstDataInQueue(InteractionData);

		if (InteractionData.bRequireLineOfSight)
		{
			SortByWeight();
		}

		if (IsInteractionTimerActive())
		{
			StopInteraction();
		}
		return;
	}

	auto Predicate = [&](const FInteractionData& Data) { return Data.Actor == Actor; };
	InteractionData = *InteractionQueue.FindByPredicate(Predicate);

	if (InteractionData.bRequireLineOfSight)
	{
		if (IsInteractionTimerActive() && ActorInSight != InteractionData.Actor)
		{
			StopInteraction();
		}

		const int32 Index = InteractionQueue.IndexOfByPredicate(Predicate);
		InteractionQueue.Swap(Index, 0);
	}
}

bool UInteractionQueueComponent::StartInteractionTimer(const FInteractionData& InteractionData)
{
	if (!GetWorld())
	{
		return false;
	}

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(InteractionTimer))
	{
		return false;
	}

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, "InteractWrapper", InteractionData);
	TimerManager.SetTimer(InteractionTimer, TimerDelegate, InteractionData.InteractionTime, false);
	OnInteractionStarted.Broadcast();
	return true;
}

void UInteractionQueueComponent::InteractWrapper(const FInteractionData& InteractionData) const
{
	Interact(InteractionData);
}

bool UInteractionQueueComponent::IsInteractionTimerActive() const
{
	if (!GetWorld())
	{
		return false;
	}

	const FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	return TimerManager.IsTimerActive(InteractionTimer);
}
