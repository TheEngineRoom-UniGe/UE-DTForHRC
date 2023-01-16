// Copyright 2020, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CheckpointHandler.h"
#include "CheckpointEventBroadcaster.h"
#include "UCheckpointComponent.generated.h"


UCLASS( ClassGroup=(Checkpoint), meta=(BlueprintSpawnableComponent) )
class UCHECKPOINTSYSTEM_API UCheckpointComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCheckpointComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

        UCheckpointEventBroadcaster* CheckpointEventBroadcaster;
        UCheckpointHandler* CheckpointHandler;


    UFUNCTION(BlueprintCallable, Category=Checkpoint)
      void SaveCheckpoint(const FString& InArgs);

    /* Function that is Bound to the ResetCheckpoint broadcast of the  CheckpointEventBroadcaster
       Has to be a UFunction to be able to bind to a delegate  */
    UFUNCTION(BlueprintCallable, Category=Checkpoint)
      void ResetCheckpoint(const FString& InArgs);

    //Object that is the target of the checkpoint
    UObject* CheckpointObject;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
