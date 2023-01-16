// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointEventBroadcaster.h"

bool UCheckpointEventBroadcaster::Broadcast(FString InCommand, FString InArgs)
{
  bool bSuccess = true;
   if(InCommand.Equals(TEXT("get")))
    {
      OnGetCheckpointEvent.Broadcast(InArgs);
    }
   else if(InCommand.Equals(TEXT("set")))
    {
      OnSetCheckpointEvent.Broadcast(InArgs);
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Checkpoint command %s not supported"), *InCommand);
      bSuccess = false;
    }
  return bSuccess;
}
