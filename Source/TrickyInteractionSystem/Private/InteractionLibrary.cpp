// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov


#include "InteractionLibrary.h"

#include "GameFramework/Character.h"

bool UInteractionLibrary::GetPlayerViewpoint(const AActor* Actor, FVector& ViewLocation, FRotator& ViewRotation)
{
	const ACharacter* Character = Cast<ACharacter>(Actor);

	if (!Character)
	{
		return  false;
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
