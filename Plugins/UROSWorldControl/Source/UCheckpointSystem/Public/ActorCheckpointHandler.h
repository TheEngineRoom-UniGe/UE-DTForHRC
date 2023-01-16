// Copyright 2020, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann


#pragma once

#include "CoreMinimal.h"
#include "CheckpointHandler.h"
#include "ActorCheckpointSaveGame.h"
#include "ActorCheckpointHandler.generated.h"

/**
 *
 */
UCLASS()
class UCHECKPOINTSYSTEM_API UActorCheckpointHandler : public UCheckpointHandler
{
	GENERATED_BODY()

public:

    virtual USaveGame* CreateSaveGameFile() override;
    virtual void GetContent() override;
    virtual void ResetCheckpoint() override;
protected:
    virtual void SetActorState(UActorCheckpointSaveGame* InActorState);
    // virtual void ResetCheckpoint(const UObject* InSavedObject) override;
};
