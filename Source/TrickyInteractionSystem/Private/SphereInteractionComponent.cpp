// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov


#include "SphereInteractionComponent.h"

USphereInteractionComponent::USphereInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UPrimitiveComponent::SetCollisionObjectType(ECC_WorldDynamic);
	UPrimitiveComponent::SetCollisionResponseToAllChannels(ECR_Ignore);
	UPrimitiveComponent::SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void USphereInteractionComponent::SetInteractionData(const FInteractionData& Value)
{
	InteractionData = Value;
}

void USphereInteractionComponent::SetInteractionMessage(AActor* Actor, const FString& Message)
{
	InteractionData.InteractionMessage = Message;
	UInteractionLibrary::UpdateInteractionMessage(Actor, GetOwner(), Message);
}

FInteractionData USphereInteractionComponent::GetInteractionData() const
{
	return InteractionData;
}

void USphereInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &USphereInteractionComponent::HandleBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &USphereInteractionComponent::HandleEndOverlap);
}

void USphereInteractionComponent::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
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

void USphereInteractionComponent::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent,
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
