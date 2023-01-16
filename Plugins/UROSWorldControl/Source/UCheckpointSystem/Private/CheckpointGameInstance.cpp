// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointGameInstance.h"
#include "CheckpointServiceServer.h"

void UCheckpointGameInstance::OnStart()
{
  Super::OnStart();

  if(ROSHandler.IsValid())
    {
      UWorld* World = GetWorld();
      CheckpointEventBroadcaster = NewObject<UCheckpointEventBroadcaster>(this);
      CheckpointHandler = NewObject<UWorldStateCheckpointHandler>(this);


      // Initialize the CheckpointHandler
      CheckpointHandler->Init();

      // Binds the OnGetCheckpointEvent to the ResetCheckpoint function.
      CheckpointEventBroadcaster->OnGetCheckpointEvent.AddDynamic(this, &UCheckpointGameInstance::ResetCheckpoint);

      // Binds the OnSetCheckpointEvent to the SaveCheckpoint function.
      CheckpointEventBroadcaster->OnSetCheckpointEvent.AddDynamic(this, &UCheckpointGameInstance::SaveCheckpoint);

      // Service that is called to control the checkpoint system
      ROSHandler->AddServiceServer(MakeShareable<FCheckpointServiceServer>(new FCheckpointServiceServer(FString("UnrealSim"), TEXT("checkpoint"), CheckpointEventBroadcaster)));

    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("GameInstance: Handler not valid"));
    }
}

void UCheckpointGameInstance::SaveCheckpoint(const FString& InArgs)
{
  CheckpointHandler->SaveCheckpoint(GetWorld());
}

void UCheckpointGameInstance::ResetCheckpoint(const FString& InArgs)
{
  CheckpointHandler->ResetCheckpoint(GetWorld());
}
