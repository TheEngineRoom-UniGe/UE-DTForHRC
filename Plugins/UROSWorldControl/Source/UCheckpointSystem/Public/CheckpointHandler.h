// Copyright 2020, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann


#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WorldState.h"
#include "CheckpointHandler.generated.h"

/**
 *
 */
UCLASS()
class UCHECKPOINTSYSTEM_API UCheckpointHandler : public UObject
{
	GENERATED_BODY()

public:
    void Init();

        // Save the checkpoint
    // virtual void SaveCheckpoint(const UObject* InSavedObject);
    virtual void SaveCheckpoint(UObject* InSavedObject);

        // Get the checkpoint
    virtual void ResetCheckpoint(UObject* InSavedObject);

protected:

    // Saves the Game. Independent of SaveGame type
    void SaveToFile();

    virtual void ResetCheckpoint();
    // Creates the SaveGame file. Should be overridden by inherited class to generate a
    // specific type of SaveGame
    virtual USaveGame* CreateSaveGameFile();

    // Gets the Content from the SaveObject. Has to be implemented from child class
    virtual void GetContent(){
      UE_LOG(LogTemp, Error, TEXT("GetContent of CheckpointHandler not implemented"));
    };

    // Object that contains the information to be saved
    UObject* SavedObject;

    FString FileName;
    USaveGame* SaveGame;
};

UCLASS()
class UCHECKPOINTSYSTEM_API UWorldStateCheckpointHandler : public UCheckpointHandler
{
	GENERATED_BODY()
public:

        //creates a UWorldState SaveGame
    virtual USaveGame* CreateSaveGameFile() override;
    virtual void GetContent() override;

    virtual void ResetCheckpoint() override;
    // virtual void ResetCheckpoint(const UObject* InSavedObject) override;

};
