// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov


#include "InteractionQueueComponent.h"

#include "InteractionInterface.h"

UInteractionQueueComponent::UInteractionQueueComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UInteractionQueueComponent::TickComponent(float DeltaTime,
                                               ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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

bool UInteractionQueueComponent::Interact()
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

	return IInteractionInterface::Execute_ProcessInteraction(InteractionData.Actor, GetOwner());
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
	if (InteractionQueue.Num() <= 1 || !bSortByWeight)
	{
		return;
	}

	auto Predicate = [&](const FInteractionData& Data_A, const FInteractionData& Data_B)
	{
		return Data_A.SortWeight >= Data_B.SortWeight;
	};

	InteractionQueue.Sort(Predicate);
}
