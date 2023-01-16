// Copyright 2020, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann


#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeGameInstance.h"
#include "CheckpointEventBroadcaster.h"
#include "CheckpointHandler.h"
#include "WorldState.h"
#include "CheckpointGameInstance.generated.h"

/**
   Example class for using the checkpoint system
 */
UCLASS()
class UCHECKPOINTSYSTEM_API UCheckpointGameInstance : public UROSBridgeGameInstance
{
    GENERATED_BODY()

      //Called on BeginPlay setting ub the CheckpointSystem
    virtual void OnStart() override;

public:
    UCheckpointEventBroadcaster* CheckpointEventBroadcaster;
protected:

    TSharedPtr<FROSBridgeSrvServer> Server;

    UCheckpointHandler* CheckpointHandler;

    /* Function that is Bound to the SaveCheckpoint broadcast of the  CheckpointEventBroadcaster
       Has to be a UFunction to be able to bind to a delegate
     */
    UFUNCTION(BlueprintCallable, Category=Checkpoint)
      void SaveCheckpoint(const FString& InArgs);

    /* Function that is Bound to the ResetCheckpoint broadcast of the  CheckpointEventBroadcaster
       Has to be a UFunction to be able to bind to a delegate
     */
    UFUNCTION(BlueprintCallable, Category=Checkpoint)
      void ResetCheckpoint(const FString& InArgs);
};
