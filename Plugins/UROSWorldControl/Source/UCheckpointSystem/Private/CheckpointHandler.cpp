// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointHandler.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UCheckpointComponent.h"

//
void UCheckpointHandler::Init()
{
  FileName = GetName();
  SavedObject = nullptr;
  SaveGame = nullptr;
}

void UCheckpointHandler::SaveCheckpoint(UObject* InSavedObject){

  SavedObject = InSavedObject;
  if(SavedObject)
    {
      // Create or Open SaveFile
      if(UGameplayStatics::DoesSaveGameExist(*FileName, 0))
        {
          SaveGame = UGameplayStatics::LoadGameFromSlot(*FileName, 0);
        }
      else
        {
          SaveGame = CreateSaveGameFile();
        }

      // Get the content of the SavedObject
      GetContent();
      SaveToFile();

      //reset the SavedObject
      SavedObject = nullptr;

      //reset the SaveGame
      SaveGame = nullptr;
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Content is empty"));
    }
}

// Create a save game with the type the checkpoint handler is intended to be used with
USaveGame* UCheckpointHandler::CreateSaveGameFile()
{
  return UGameplayStatics::CreateSaveGameObject(USaveGame::StaticClass());
}

void UCheckpointHandler::SaveToFile()
{
  if(!UGameplayStatics::SaveGameToSlot(SaveGame, *FileName, 0))
    {
      UE_LOG(LogTemp, Log, TEXT("Saving checkpoint failed"));
    }
}

void UCheckpointHandler::ResetCheckpoint(UObject* InSavedObject)
{
  SavedObject = InSavedObject;
  if(InSavedObject)
    {
      ResetCheckpoint();
    }
  else
    {
      UE_LOG(LogTemp, Log, TEXT("ResetCheckpoint: InSavedObject is nullptr"));
    }
}
void UCheckpointHandler::ResetCheckpoint()
{
  SaveGame = UGameplayStatics::LoadGameFromSlot(*FileName, 0);
}

USaveGame* UWorldStateCheckpointHandler::CreateSaveGameFile()
{
  return UGameplayStatics::CreateSaveGameObject(UWorldState::StaticClass());
}

void UWorldStateCheckpointHandler::GetContent()
{
  TArray<AActor*> WorldActors;
  UWorldState* WorldState = Cast<UWorldState>(SaveGame);
  UGameplayStatics::GetAllActorsOfClass(SavedObject, AStaticMeshActor::StaticClass(), WorldActors);
  for(auto& Actor : WorldActors)
    {
      TArray<UCheckpointComponent*> CheckpointHandlerComponents;
      Actor->GetComponents<UCheckpointComponent>(CheckpointHandlerComponents);
      if(CheckpointHandlerComponents.Num() == 0)
        {
          WorldState->AddActor(Cast<AStaticMeshActor>(Actor));
        }
    }
}

void UWorldStateCheckpointHandler::ResetCheckpoint()
// void UWorldStateCheckpointHandler::ResetCheckpoint(const UObject* InSavedObject)
{
  Super::ResetCheckpoint();
  //TODO remove actors that are not in checkpoint file but in the world
  if(SavedObject)
    {
      UWorldState* WorldState = Cast<UWorldState>(SaveGame);
      for(auto& ActorRef : WorldState->WorldStateMap)
        {
          UStaticMeshComponent* RootComponent = Cast<UStaticMeshComponent>(ActorRef.Key->GetRootComponent());
          ActorRef.Key->SetActorTransform(ActorRef.Value.Pose);
          RootComponent->SetPhysicsLinearVelocity(ActorRef.Value.AngularVel);
          RootComponent->SetPhysicsAngularVelocityInDegrees(ActorRef.Value.LinearVel);
        }
    }
}
