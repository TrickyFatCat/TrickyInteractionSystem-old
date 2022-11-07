// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov


#include "InteractionTriggers/BoxInteractionTrigger.h"

UBoxInteractionTrigger::UBoxInteractionTrigger()
{
	PrimaryComponentTick.bCanEverTick = true;


	UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UPrimitiveComponent::SetCollisionObjectType(ECC_WorldDynamic);
	UPrimitiveComponent::SetCollisionResponseToAllChannels(ECR_Ignore);
	UPrimitiveComponent::SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void UBoxInteractionTrigger::SetInteractionSettings(const FInteractionData& Value)
{
	InteractionSettings = Value;
	InteractionSettings.Actor = GetOwner();
}

void UBoxInteractionTrigger::SetInteractionMessage(const FString& Message)
{
	InteractionSettings.InteractionMessage = Message;
}

FInteractionData UBoxInteractionTrigger::GetInteractionSettings() const
{
	return InteractionSettings;
}

void UBoxInteractionTrigger::BeginPlay()
{
	Super::BeginPlay();

	InteractionSettings.Actor = GetOwner();
	OnComponentBeginOverlap.AddDynamic(this, &UBoxInteractionTrigger::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UBoxInteractionTrigger::OnEndOverlap);
}

void UBoxInteractionTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                            AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp,
                                            int32 OtherBodyIndex,
                                            bool bFromSweep,
                                            const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && !bIsNormalTrigger)
	{
		UInteractionLibrary::AddDataToQueue(OtherActor, InteractionSettings);
	}
}

void UBoxInteractionTrigger::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                          AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp,
                                          int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && !bIsNormalTrigger)
	{
		UInteractionLibrary::RemoveDataFromQueue(OtherActor, InteractionSettings);
	}
}
