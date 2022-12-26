// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov


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

void UBoxInteractionComponent::SetInteractionMessage(AActor* Actor, const FString& Message)
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

	OnComponentBeginOverlap.AddDynamic(this, &UBoxInteractionComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UBoxInteractionComponent::OnEndOverlap);
}

void UBoxInteractionComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                              AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp,
                                              int32 OtherBodyIndex,
                                              bool bFromSweep,
                                              const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		UInteractionLibrary::AddToInteractionQueue(OtherActor, GetOwner(), InteractionData);
	}
}

void UBoxInteractionComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                            AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp,
                                            int32 OtherBodyIndex)
{
	if (IsValid(OtherActor))
	{
		UInteractionLibrary::RemoveFromInteractionQueue(OtherActor, GetOwner());
	}
}
