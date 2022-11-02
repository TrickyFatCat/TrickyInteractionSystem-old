// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov


#include "InteractionTriggers/SphereInteractionTrigger.h"

#include "InteractionLibrary.h"

USphereInteractionTrigger::USphereInteractionTrigger()
{
	PrimaryComponentTick.bCanEverTick = true;


	UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UPrimitiveComponent::SetCollisionObjectType(ECC_WorldDynamic);
	UPrimitiveComponent::SetCollisionResponseToAllChannels(ECR_Ignore);
	UPrimitiveComponent::SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void USphereInteractionTrigger::SetInteractionSettings(const FInteractionData& Value)
{
	InteractionSettings = Value;
	InteractionSettings.Actor = GetOwner();
}

FInteractionData USphereInteractionTrigger::GetInteractionSettings() const
{
	return InteractionSettings;
}

void USphereInteractionTrigger::BeginPlay()
{
	Super::BeginPlay();

	InteractionSettings.Actor = GetOwner();
	OnComponentBeginOverlap.AddDynamic(this, &USphereInteractionTrigger::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &USphereInteractionTrigger::OnEndOverlap);
}

void USphereInteractionTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
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

void USphereInteractionTrigger::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                             AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp,
                                             int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && !bIsNormalTrigger)
	{
		UInteractionLibrary::RemoveDataFromQueue(OtherActor, InteractionSettings);
	}
}
