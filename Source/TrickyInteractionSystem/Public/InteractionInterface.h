// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * An interface used to implement the interaction functionality.
 */
class TRICKYINTERACTIONSYSTEM_API IInteractionInterface
{
	GENERATED_BODY()

public:
	/**
	 * This function called when the interaction queue component starts the interaction.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	void StartInteraction(AActor* OtherActor);

	/**
	 * This function called to activate the interaction effect returns true if success, else false.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	bool FinishInteraction(AActor* OtherActor);

	/**
	 * This function called when the StopInteraction called from the interaction queue component.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	void StopInteraction(AActor* OtherActor);
};
