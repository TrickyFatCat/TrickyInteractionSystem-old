// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov


#include "InteractionQueueComponent.h"

#include "InteractionLibrary.h"
#include "InteractionInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "Engine/World.h"

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

bool UInteractionQueueComponent::Add(AActor* Actor, const FInteractionData& InteractionData)
{
	if (!IsValid(Actor) && QueueHasActor(Actor) && !UInteractionLibrary::HasInteractionInterface(Actor))
	{
		return false;
	}

	InteractionQueue.Add(FQueueData{Actor, InteractionData});
	SortByWeight();
	OnActorAdded.Broadcast(Actor);
	return true;
}

bool UInteractionQueueComponent::Remove(const AActor* Actor)
{
	if (!QueueHasActor(Actor))
	{
		return false;
	}

	auto Predicate = [&](const FQueueData& Data) { return Data.Actor == Actor; };
	const bool bItemsRemoved = InteractionQueue.RemoveAll(Predicate) > 0;

	if (bItemsRemoved)
	{
		SortByWeight();
		OnActorRemoved.Broadcast(Actor);
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
	GetFirstInteractionData(InteractionData);
	AActor* Actor = GetFirstActor();

	if (!IsValid(Actor))
	{
		return false;
	}

	if (!UInteractionLibrary::HasInteractionInterface(Actor))
	{
		LogWarning(FString::Printf(TEXT("Actor %s doesn't have InteractionInterface implemented."),
		                           *Actor->GetClass()->GetName()));
		return false;
	}

	if (InteractionData.bRequireLineOfSight && Actor != ActorInSight)
	{
		return false;
	}

	if (InteractionData.InteractionTime > 0.f)
	{
		return StartInteractionTimer(InteractionQueue[0]);
	}

	OnInteractionStarted.Broadcast(Actor);
	IInteractionInterface::Execute_StartInteraction(Actor, GetOwner());

	return bFinishManually ? true : FinishInteraction(GetFirstActor());
}

bool UInteractionQueueComponent::FinishInteraction(AActor* Actor)
{
	bool bResult = false;

	if (!QueueHasActor(Actor))
	{
		return bResult;
	}

	FInteractionData InteractionData;
	GetInteractionData(Actor, InteractionData);

	if (InteractionData.bRequireLineOfSight && Actor != ActorInSight)
	{
		return bResult;
	}

	bResult = IInteractionInterface::Execute_FinishInteraction(Actor, GetOwner());

	if (bResult)
	{
		OnInteractionFinishedSignature.Broadcast(Actor);
		return bResult;
	}

	return bResult;
}

bool UInteractionQueueComponent::StopInteraction()
{
	if (!GetWorld())
	{
		return false;
	}

	FInteractionData InteractionData;
	GetFirstInteractionData(InteractionData);
	AActor* Actor = GetFirstActor();

	if (!IsValid(Actor))
	{
		return false;
	}

	if (!UInteractionLibrary::HasInteractionInterface(Actor))
	{
		LogWarning(FString::Printf(TEXT("Actor %s doesn't have InteractionInterface implemented."),
		                           *Actor->GetClass()->GetName()));
		return false;
	}

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(InteractionTimer))
	{
		TimerManager.ClearTimer(InteractionTimer);
	}

	OnInteractionStopped.Broadcast(Actor);
	IInteractionInterface::Execute_StopInteraction(Actor, GetOwner());
	return true;
}

bool UInteractionQueueComponent::IsQueueEmpty() const
{
	return InteractionQueue.Num() == 0;
}

bool UInteractionQueueComponent::QueueHasActor(const AActor* Actor) const
{
	if (!IsValid(Actor))
	{
		return false;
	}

	auto Predicate = [&](const FQueueData& Data) { return Data.Actor == Actor; };
	return InteractionQueue.ContainsByPredicate(Predicate);
}

void UInteractionQueueComponent::GetFirstInteractionData(FInteractionData& Data)
{
	if (IsQueueEmpty())
	{
		return;
	}

	Data = InteractionQueue[0].InteractionData;
}

AActor* UInteractionQueueComponent::GetFirstActor()
{
	if (IsQueueEmpty())
	{
		return nullptr;
	}

	return InteractionQueue[0].Actor;
}

bool UInteractionQueueComponent::GetFirstQueueData(FQueueData& QueueData)
{
	if (IsQueueEmpty())
	{
		return false;
	}

	QueueData = InteractionQueue[0];
	return true;
}

bool UInteractionQueueComponent::GetInteractionData(const AActor* Actor, FInteractionData& Data)
{
	if (!QueueHasActor(Actor))
	{
		return false;
	}

	Data = FindInteractionData(Actor);
	return true;
}

bool UInteractionQueueComponent::UpdateInteractionMessage(const AActor* Actor, const FString& NewMessage)
{
	if (!QueueHasActor(Actor))
	{
		return false;
	}

	FindInteractionData(Actor).InteractionMessage = NewMessage;
	return true;
}

bool UInteractionQueueComponent::UpdateInteractionTime(const AActor* Actor, const float NewTime)
{
	if (!QueueHasActor(Actor))
	{
		return false;
	}

	FindInteractionData(Actor).InteractionTime = NewTime < 0.f ? 0.f : NewTime;
	return true;
}

void UInteractionQueueComponent::SortByWeight()
{
	if (InteractionQueue.Num() <= 1)
	{
		return;
	}

	auto PredicateWeight = [&](const FQueueData& Data_A, const FQueueData& Data_B)
	{
		return Data_A.InteractionData.SortWeight >= Data_B.InteractionData.SortWeight &&
				Data_A.InteractionData.bRequireLineOfSight <= Data_B.InteractionData.bRequireLineOfSight;
	};

	InteractionQueue.Sort(PredicateWeight);
}

void UInteractionQueueComponent::LogWarning(const FString& Message) const
{
	if (!GetWorld())
	{
		return;
	}

	const FString ErrorMessage{FString::Printf(TEXT("%s | Actor: %s"), *Message, *GetOwner()->GetName())};
	UE_LOG(LogInteractionQueueComponent, Warning, TEXT("%s"), *ErrorMessage);
}

FInteractionData& UInteractionQueueComponent::FindInteractionData(const AActor* Actor)
{
	auto Predicate = [&](const FQueueData& QueueData) { return QueueData.Actor == Actor; };
	return InteractionQueue.FindByPredicate(Predicate)->InteractionData;
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
		GetFirstInteractionData(InteractionData);

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

	auto Predicate = [&](const FQueueData& Data) { return Data.Actor == Actor; };
	const FQueueData QueueData = *InteractionQueue.FindByPredicate(Predicate);
	InteractionData = QueueData.InteractionData;

	if (InteractionData.bRequireLineOfSight)
	{
		if (IsInteractionTimerActive() && ActorInSight != QueueData.Actor)
		{
			StopInteraction();
		}

		const int32 Index = InteractionQueue.IndexOfByPredicate(Predicate);
		InteractionQueue.Swap(Index, 0);
	}
}

bool UInteractionQueueComponent::StartInteractionTimer(const FQueueData& QueueData)
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

	const FInteractionData InteractionData = QueueData.InteractionData;

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, "FinishInteractionWrapper", QueueData.Actor);
	TimerManager.SetTimer(InteractionTimer, TimerDelegate, InteractionData.InteractionTime, false);

	OnInteractionStarted.Broadcast(QueueData.Actor);
	IInteractionInterface::Execute_StartInteraction(QueueData.Actor, GetOwner());

	return true;
}

void UInteractionQueueComponent::FinishInteractionWrapper(AActor* Actor)
{
	FinishInteraction(Actor);
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
