// Copyright 2020, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann



#include "ActorCheckpointHandler.h"
#include "Kismet/GameplayStatics.h"


USaveGame* UActorCheckpointHandler::CreateSaveGameFile()
{
  return UGameplayStatics::CreateSaveGameObject(UActorCheckpointSaveGame::StaticClass());
}

void UActorCheckpointHandler::GetContent()
{
  UActorCheckpointSaveGame* ActorState = Cast<UActorCheckpointSaveGame>(SaveGame);
  if(ActorState)
    {
      ActorState->ExtractInformation(Cast<AActor>(SavedObject));
    }

  UE_LOG(LogTemp, Error, TEXT("ActorCheckpointHandler Content Extracted"));
}

// void UActorCheckpointHandler::ResetCheckpoint(const UObject* InSavedObject)
void UActorCheckpointHandler::ResetCheckpoint()
{
  Super::ResetCheckpoint();
  UActorCheckpointSaveGame* ActorState = Cast<UActorCheckpointSaveGame>(SaveGame);
  SetActorState(ActorState);
}

void UActorCheckpointHandler::SetActorState(UActorCheckpointSaveGame* InActorState)
{
  AActor* Actor = Cast<AActor>(SavedObject);
  if(Actor)
    {
      UPrimitiveComponent* RootComponent = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
      if(RootComponent)
        {
          Actor->SetActorTransform(InActorState->ActorState.Pose);
          RootComponent->SetPhysicsLinearVelocity(InActorState->ActorState.AngularVel);
          RootComponent->SetPhysicsAngularVelocityInDegrees(InActorState->ActorState.LinearVel);
        }
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("ResetCheckpoint: SavedObject can not be cast AActor"));
    }
}
