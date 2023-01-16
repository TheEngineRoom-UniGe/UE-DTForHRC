// Copyright 2020, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann



#include "ActorCheckpointSaveGame.h"


void UActorCheckpointSaveGame::ExtractInformation(const AActor* InActor)
{
  if(InActor)
    {
      UPrimitiveComponent* RootComponent = Cast<UPrimitiveComponent>(InActor->GetRootComponent());
      if(RootComponent)
        {
          ActorState.Pose = InActor->GetActorTransform();
          ActorState.LinearVel = InActor->GetVelocity();
          ActorState.AngularVel = RootComponent->GetPhysicsAngularVelocityInDegrees();
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("Root Component of %s is not of type UPrimtivecomponent"), *InActor->GetName());
        }
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("InActor of %s is nullptr"), *GetName());
    }
}
