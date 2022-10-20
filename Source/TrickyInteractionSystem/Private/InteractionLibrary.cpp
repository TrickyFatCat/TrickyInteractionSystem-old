// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov


#include "InteractionLibrary.h"

#include "InteractionQueueComponent.h"
#include "GameFramework/Character.h"

bool UInteractionLibrary::GetPlayerViewpoint(const AActor* Actor, FVector& ViewLocation, FRotator& ViewRotation)
{
	const ACharacter* Character = Cast<ACharacter>(Actor);

	if (!Character)
	{
		return false;
	}

	if (Character->IsPlayerControlled())
	{
		const APlayerController* Controller = Character->GetController<APlayerController>();

		if (!Controller)
		{
			return false;
		}

		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}

	return true;
}

bool UInteractionLibrary::AddDataToQueue(const AActor* Actor, const FInteractionData& InteractionData)
{
	if (!IsValid(Actor) || !IsValid(InteractionData.Actor))
	{
		return false;
	}

	UInteractionQueueComponent* InteractionQueueComponent = Actor->FindComponentByClass<UInteractionQueueComponent>();

	if (!InteractionQueueComponent)
	{
		return false;
	}

	return InteractionQueueComponent->Add(InteractionData);
}

bool UInteractionLibrary::RemoveDataFromQueue(const AActor* Actor, const FInteractionData& InteractionData)
{
	if (!IsValid(Actor) || !IsValid(InteractionData.Actor))
	{
		return false;
	}

	UInteractionQueueComponent* InteractionQueueComponent = Actor->FindComponentByClass<UInteractionQueueComponent>();
	
	if (!InteractionQueueComponent)
	{
		return false;
	}

	return InteractionQueueComponent->Remove(InteractionData);
}

bool UInteractionLibrary::AddToQueue(const AActor* TargetActor,
                                     AActor* InteractiveActor,
                                     const bool bRequireLineOfSight,
                                     const FString& InteractionMessage,
                                     const int32 SortWeight)
{
	if (!IsValid(InteractiveActor) || !IsValid(TargetActor))
	{
		return false;
	}

	const FInteractionData InteractionData{InteractiveActor, bRequireLineOfSight, InteractionMessage, SortWeight};
	return AddDataToQueue(TargetActor, InteractionData);
}

bool UInteractionLibrary::RemoveFromQueue(const AActor* TargetActor, const AActor* InteractiveActor)
{
	if (!IsValid(TargetActor) || !IsValid(InteractiveActor))
	{
		return false;
	}

	UInteractionQueueComponent* InteractionQueueComponent = TargetActor->FindComponentByClass<UInteractionQueueComponent>();
	
	if (!InteractionQueueComponent)
	{
		return false;
	}

	return InteractionQueueComponent->RemoveActor(InteractiveActor);
}
