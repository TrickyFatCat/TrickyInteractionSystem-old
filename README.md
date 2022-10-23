# TrickyInteractionSystem

An Unreal Engine plugin contains components for integration of the simple interaction system in a prototype.

## Installation

The plugin can be used in both C++ and Blueprint projects.

### Blueprint projects

**At the moment, the package is compatible only with Unreal Engine 4.**

1. Download [**package file**](https://github.com/TrickyFatCat/TrickyInteractionSystem/releases/tag/v1.0);
2. Unzip the package to the Plugins folder in engine folder, e.g. `C:\Program Files\Epic Games\UE_4.27\Engine\Plugins`;
3. Restart the project;

In this case the plugin can be used for any blueprint project.

### C++ projects

1. Create the Plugins folder in the project directory;
2. Create the TrickyAnimationComponents folder in the Plugins folder;
3. Download the plugin source code into that folder;
4. Rebuild the project;

## Content

The plugin contains:

1. InteractionQueueComponent;
2. InteractionInterface;
3. InteractionLibrary;
4. TriggerComponents;

### InteractionQueueComponent

This component handles creating a queue for interaction which it sorts by weight and line of sight.

#### Parameters

#### Functions

#### Delegates


### Interaction data

The struct which contains parameters for interaction behaviour adjustments.

### Parameters

1. `Actor` - an interactive actor with `InteractionInterface` implemented.
   * Use `SetActor` function from InteractionLibrary to set this parameter in blueprints;
   * It's better to do it in ConstructionScript or OnBeginPlay;
2. `bRequireLineOfSight` - toggles if the actor require being in the line of sight to be interacted; 
3. `InteractionMessage` - a message which can be used in HUD;
4. `SortWeight` - a sort weight for sorting the interaction queue;
5. `InteractionTime` - how much time required to activate interaction effect (call `Interact` function);

### InteractionInterface

The interface which used to implement the interaction functionality in the chosen actor.

#### Functions

1. `StartInteraction` - this function called when the interaction queue component starts the interaction;
2. `Interact` - this function called to activate the interaction effect returns `true` if success, else `false`;
3. `StopInteraction` - this function called when the `StopInteraction` called from the interaction queue component;

### Interaction Library

A library which contains some useful functions for custom implementation of the system using Blueprints.

#### Functions

1. `Equal` - compare interaction data and returns if they're equal or not;
2. `SetActor` - sets the `Actor` variable in the given interaction data;
3. `GetPlayerViewpoint` - returns player's viewport location and rotation;
4. `AddToQueue` - adds custom interaction data to the interaction queue component of the given actor;
5. `RemoveFromQueue` - removes interaction data from the interaction queue component of the given actor if it was found;
6. `HasInteractionInterface` - checks if the given actor has `InteractionInterface`;

### InteractionTriggers

A collection of triggers which can add to and remove form the interaction queue the owner actor.

The plugin has:
1. BoxInteractionTrigger;
2. CapsuleInteractionTrigger;
3. SphereInteractionTrigger;

#### Parameters

1. `IsNormalTrigger` - toggles if the trigger should behave as a normal trigger;
2. `InteractionSettings` - interaction data which will be used by the interaction queue component;

## Quick Setup

1. Add InteractionQueueComponent to your character;
2. Create an interactive actor:
   * Add InteractionInterface to the chosen actor and write the logic in the `Interact` function;
   * Add any of InteractionTrigger;
3. Create "Interaction" input action;
4. Call the `StartInteraction` function of InteractionQueueComponent;