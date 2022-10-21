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

### InteractionInterface

The interface which used to implement the logic which will be called by InteractionQueueComponent.

#### Functions

### Interaction Library

A library which contains some useful functions for custom implementation of the system using Blueprints.

#### Functions

### InteractionTriggers

A collection of triggers which can add to and remove form the interaction queue the owner actor.

#### Parameters

## Quick Setup

1. Add InteractionQueueComponent to your character;
2. Create an interactive actor:
   * Add InteractionInterface to the chosen actor and write the logic in the `ProcessInteraction` function;
   * Add any of InteractionTrigger;
3. Create "Interaction" input action;
4. Call the `Interact` function of InteractionQueueComponent;