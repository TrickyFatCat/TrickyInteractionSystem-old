// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov


#include "InteractionLibrary.h"

#include "InteractionInterface.h"
#include "InteractionQueueComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

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

bool UInteractionLibrary::AddToInteractionQueue(const AActor* Actor,
                                                AActor* InteractiveActor,
                                                const FInteractionData& InteractionData)
{
	if (!IsValid(Actor) || !IsValid(InteractiveActor))
	{
		return false;
	}

	UInteractionQueueComponent* InteractionQueueComponent = Actor->FindComponentByClass<UInteractionQueueComponent>();

	if (!InteractionQueueComponent)
	{
		return false;
	}

	return InteractionQueueComponent->Add(InteractiveActor, InteractionData);
}


bool UInteractionLibrary::RemoveFromInteractionQueue(const AActor* Actor, const AActor* InteractiveActor)
{
	if (!IsActorInInteractionQueue(Actor, InteractiveActor))
	{
		return false;
	}

	UInteractionQueueComponent* InteractionQueueComponent = Actor->FindComponentByClass<UInteractionQueueComponent>();

	if (!InteractionQueueComponent)
	{
		return false;
	}

	return InteractionQueueComponent->Remove(InteractiveActor);
}

bool UInteractionLibrary::HasInteractionInterface(const AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return false;
	}
	
	return Actor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass());
}

bool UInteractionLibrary::IsActorInInteractionQueue(const AActor* Actor, const AActor* InteractiveActor)
{
	if (!IsValid(Actor))
	{
		return false;
	}

	const UInteractionQueueComponent* InteractionQueueComponent = Actor->FindComponentByClass<
		UInteractionQueueComponent>();

	if (!InteractionQueueComponent)
	{
		return false;
	}

	return InteractionQueueComponent->QueueHasActor(InteractiveActor);
}

bool UInteractionLibrary::GetInteractionData(const AActor* Actor,
                                             const AActor* InteractiveActor,
                                             FInteractionData& InteractionData)
{
	if (!IsActorInInteractionQueue(Actor, InteractiveActor))
	{
		return false;
	}

	UInteractionQueueComponent* InteractionQueueComponent = Actor->FindComponentByClass<UInteractionQueueComponent>();

	if (!InteractionQueueComponent)
	{
		return false;
	}

	return InteractionQueueComponent->GetInteractionData(InteractiveActor, InteractionData);
}
