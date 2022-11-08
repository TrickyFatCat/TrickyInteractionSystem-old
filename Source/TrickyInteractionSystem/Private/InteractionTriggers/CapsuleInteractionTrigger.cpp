// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov


#include "InteractionTriggers/CapsuleInteractionTrigger.h"

UCapsuleInteractionTrigger::UCapsuleInteractionTrigger()
{
	PrimaryComponentTick.bCanEverTick = true;


	UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UPrimitiveComponent::SetCollisionObjectType(ECC_WorldDynamic);
	UPrimitiveComponent::SetCollisionResponseToAllChannels(ECR_Ignore);
	UPrimitiveComponent::SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void UCapsuleInteractionTrigger::SetInteractionSettings(const FInteractionData& Value)
{
	InteractionSettings = Value;
}

void UCapsuleInteractionTrigger::SetInteractionMessage(const FString& Message)
{
	InteractionSettings.InteractionMessage = Message;
}

FInteractionData UCapsuleInteractionTrigger::GetInteractionSettings() const
{
	return InteractionSettings;
}

void UCapsuleInteractionTrigger::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UCapsuleInteractionTrigger::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UCapsuleInteractionTrigger::OnEndOverlap);
}

void UCapsuleInteractionTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp,
                                                int32 OtherBodyIndex,
                                                bool bFromSweep,
                                                const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && !bIsNormalTrigger)
	{
		UInteractionLibrary::AddToInteractionQueue(OtherActor, GetOwner(), InteractionSettings);
	}
}

void UCapsuleInteractionTrigger::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                              AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp,
                                              int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && !bIsNormalTrigger)
	{
		UInteractionLibrary::RemoveFromInteractionQueue(OtherActor, GetOwner());
	}
}
