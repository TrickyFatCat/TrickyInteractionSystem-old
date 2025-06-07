# TrickyInteractionSystem

This projects isn't in development no more. You can find a new version [**here**](https://github.com/TrickyFatCat/TrickyInteractionSystem)

An Unreal Engine plugin contains components for integration of the simple interaction system in a prototype.

## Installation

The plugin can be used in both C++ and Blueprint projects.

### Blueprint projects

1. Download the latest [**package file**](https://github.com/TrickyFatCat/TrickyInteractionSystem/releases/);
2. Install the plugin either in your project or for all projects in engine:
   * Unzip the package into Plugins folder of your project, e.g. `D:\UnrealProjects\MyProject\Plugins`;
   * Unzip the package to the Plugins folder in engine folder, e.g. `C:\Program Files\Epic Games\UE_5.0\Engine\Plugins`;
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
4. Interaction Triggers;

### InteractionQueueComponent

This component handles creating a queue for interaction which it sorts by weight and line of sight.

#### Parameters

1. `InteractionQueue` - current interaction queue;
   * It's read only and exposed for debug purposes;
2. `FinishManually` - if true the `FinishInteraction` function must be called manually through the code;
3. `UseLineOfSight` - toggles the line of sight checks;
   * Keep it false if there's no interactive actors require line of sight checks in your game;
4. `TraceChannel` - the trace channel used for line of sight checks;
   * Currently read only;
5. `SightDistance` - the line of sight max distance;
6. `SightRadius` - the line of sight radius;
7. `ActorInSight` - the actor caught by line of sight;

#### Functions

1. `Add` - adds interaction data into the interaction queue;
2. `Remove` - removes interaction data from the interaction queue;
3. `StartInteraction` - starts interaction with the first actor in the interaction queue;
4. `FinishInteraction` - finishes the interaction action. Call it if '
5. `StopInteraction` - stops interaction;
6. `IsQueueEmpty` - checks if the interaction queue is empty;
7. `QueueHasActor` - checks if the interaction queue has interaction data with the given actor;
8. `GetFirstInteractionData` - returns the first interaction data in the interaction queue;
9. `GetFirstActor` - returns the first actor in the queue;
10. `GetFirstDataInQueue` - returns the first interaction data in queue;
11. `GetInteractionData` - returns the interaction data of the given actor;
12. `UpdateInteractionMessage` - updates interaction message of the given actor in the interaction queue;
13. `UpdateInteractionTime` - updates interaction time of the given actor in the queue; 

#### Delegates

1. `OnInteractionStarted` - called when the interaction process started;
2. `OnInteract` - called when the interaction effect successfully activated;
3. `OnInteractionStopped` - called when the interaction process stopped;
4. `OnActorAdded` - called when a new actor was added to the queue;
5. `OnActorRemoved` - called when an actor was removed from the queue;

### Interaction data

The struct which contains parameters for interaction behaviour adjustments.

### Parameters

1. `bRequireLineOfSight` - toggles if the actor require being in the line of sight to be interacted; 
2. `InteractionMessage` - a message which can be used in HUD;
3. `SortWeight` - a sort weight for sorting the interaction queue;
4. `InteractionTime` - how much time required to activate interaction effect (call `Interact` function);

### InteractionInterface

The interface which used to implement the interaction functionality in the chosen actor.

#### Functions

1. `StartInteraction` - this function called when the interaction queue component starts the interaction;
2. `FinishInteraction` - this function called to activate the interaction effect returns `true` if success, else `false`;
3. `StopInteraction` - this function called when the `StopInteraction` called from the interaction queue component;

### Interaction Library

A library which contains some useful functions for custom implementation of the system using Blueprints.

#### Functions

1. `GetPlayerViewpoint` - returns player's viewport location and rotation;
2. `GetInteractionComponent` - returns InteractionQueueComponent if the given actor has one;
3. `IsInteractionQueueEmpty` - checks if the interaction queue is empty;
4. `AddToInteractionQueue` - adds custom interaction data to the interaction queue component of the given actor;
5. `RemoveFromInteractionQueue` - removes interaction data from the interaction queue component of the given actor if it was found;
6. `HasInteractionInterface` - checks if the given actor has `InteractionInterface`;
7. `InteractionQueueHasActor` - checks if the interaction queue has a given actor in it;
8. `GetInteractionData` - returns interaction data of the given actor;
9. `GetFirstQueueData` - return the first data in the queue;
10. `UpdateInteractionMessage` - updates the interaction message of the given interactive actor;

## Interaction Triggers

A small collection of trigger components which add and remove their owning actor from the interaction queue.

1. BoxInteractionComponent;
2. CapsuleInteractionComponent;
3. SphereInteractionComponent;

### Parameters

1. `InteractionData` - interaction data which will be added to the interaction queue;

### Functions

1. `SetInteractionMessage` - sets interaction message in the `InteractionData` and updates it in the interaction queue;

### Delegates

1. `OnActorAdded` - called when the owning actor was added to the queue;
2. `OnActorRemoved` - called when the owning actor was removed from the queue;

## Quick Setup

1. Add InteractionQueueComponent to your character;
2. Create an interactive actor:
   * Add InteractionInterface to the chosen actor and write the logic in the `Interact` function;
   * Add an interaction trigger;
   * Adjust interaction data in the trigger;
3. Create "Interaction" input action;
4. Call the `StartInteraction` function of InteractionQueueComponent;
