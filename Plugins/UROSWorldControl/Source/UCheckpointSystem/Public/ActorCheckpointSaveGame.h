// Copyright 2020, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ActorCheckpointSaveGame.generated.h"

/**
 *
 */
UCLASS()
class UCHECKPOINTSYSTEM_API UActorCheckpointSaveGame : public USaveGame
{
    GENERATED_BODY()

 public:

    UPROPERTY(VisibleAnywhere)
    FWorldStateInformation ActorState;

  virtual void ExtractInformation(const AActor* InActor);
};
