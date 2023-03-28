// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "InteractionLibrary.h"

#include "InteractionInterface.h"
#include "InteractionQueueComponent.h"
#include "Components/ShapeComponent.h"
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

UInteractionQueueComponent* UInteractionLibrary::GetInteractionQueueComponent(const AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return nullptr;
	}

	return Actor->FindComponentByClass<UInteractionQueueComponent>();
}

bool UInteractionLibrary::IsInteractionQueueEmpty(const AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return true;
	}

	const UInteractionQueueComponent* InteractionQueueComponent = Actor->FindComponentByClass<
		UInteractionQueueComponent>();

	if (!InteractionQueueComponent)
	{
		return true;
	}

	return InteractionQueueComponent->IsQueueEmpty();
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
	if (!InteractionQueueHasActor(Actor, InteractiveActor))
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

bool UInteractionLibrary::InteractionQueueHasActor(const AActor* Actor, const AActor* InteractiveActor)
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
	if (!InteractionQueueHasActor(Actor, InteractiveActor))
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

bool UInteractionLibrary::GetFirstQueueData(const AActor* Actor, FQueueData& QueueData)
{
	if (!IsValid(Actor))
	{
		return false;
	}

	UInteractionQueueComponent* InteractionQueueComponent = Actor->FindComponentByClass<UInteractionQueueComponent>();

	if (!InteractionQueueComponent)
	{
		return false;
	}

	return InteractionQueueComponent->GetFirstQueueData(QueueData);
}

bool UInteractionLibrary::UpdateInteractionMessage(const AActor* Actor,
                                                   const AActor* InteractiveActor,
                                                   const FString& NewMessage)
{
	if (!InteractionQueueHasActor(Actor, InteractiveActor))
	{
		return false;
	}

	UInteractionQueueComponent* InteractionQueueComponent = Actor->FindComponentByClass<UInteractionQueueComponent>();

	if (!InteractionQueueComponent)
	{
		return false;
	}

	InteractionQueueComponent->UpdateInteractionMessage(InteractiveActor, NewMessage);
	return true;
}

bool UInteractionLibrary::UpdateInteractionTime(const AActor* Actor,
                                                const AActor* InteractiveActor,
                                                const float NewTime)
{
	if (!IsValid(Actor) || !InteractionQueueHasActor(Actor, InteractiveActor))
	{
		return false;
	}

	UInteractionQueueComponent* InteractionQueueComponent = Actor->FindComponentByClass<UInteractionQueueComponent>();

	if (!InteractionQueueComponent)
	{
		return false;
	}

	return InteractionQueueComponent->UpdateInteractionTime(InteractiveActor, NewTime);
}

void UInteractionLibrary::SetTriggerDefaultCollision(UShapeComponent* ShapeComponent)
{
	if (!ShapeComponent)
	{
		return;
	}

	ShapeComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShapeComponent->SetCollisionObjectType(ECC_WorldDynamic);
	ShapeComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ShapeComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}
