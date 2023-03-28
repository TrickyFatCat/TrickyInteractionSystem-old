// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "BoxInteractionComponent.h"

UBoxInteractionComponent::UBoxInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UPrimitiveComponent::SetCollisionObjectType(ECC_WorldDynamic);
	UPrimitiveComponent::SetCollisionResponseToAllChannels(ECR_Ignore);
	UPrimitiveComponent::SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void UBoxInteractionComponent::SetInteractionData(const FInteractionData& Value)
{
	InteractionData = Value;
}

void UBoxInteractionComponent::SetInteractionMessage(const AActor* Actor, const FString& Message)
{
	InteractionData.InteractionMessage = Message;
	UInteractionLibrary::UpdateInteractionMessage(Actor, GetOwner(), Message);
}

FInteractionData UBoxInteractionComponent::GetInteractionData() const
{
	return InteractionData;
}

void UBoxInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UBoxInteractionComponent::HandleBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UBoxInteractionComponent::HandleEndOverlap);
}

void UBoxInteractionComponent::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                  AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp,
                                                  int32 OtherBodyIndex,
                                                  bool bFromSweep,
                                                  const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	if (UInteractionLibrary::AddToInteractionQueue(OtherActor, GetOwner(), InteractionData))
	{
		OnActorAdded.Broadcast(OtherActor);
	}
}

void UBoxInteractionComponent::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                                AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp,
                                                int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	if (UInteractionLibrary::RemoveFromInteractionQueue(OtherActor, GetOwner()))
	{
		OnActorRemoved.Broadcast(OtherActor);
	}
}
