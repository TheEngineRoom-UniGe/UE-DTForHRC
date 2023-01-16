// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldState.h"


void UWorldState::AddActor(AStaticMeshActor* InActor)
{
  FWorldStateInformation TempState;
  UStaticMeshComponent* RootComponent = Cast<UStaticMeshComponent>(InActor->GetRootComponent());
  if(RootComponent)
    {
      TempState.Pose = InActor->GetActorTransform();
      TempState.LinearVel = InActor->GetVelocity();
      TempState.AngularVel = RootComponent->GetPhysicsAngularVelocityInDegrees();
      WorldStateMap.Add(InActor, TempState);
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Root Component of %s is not of type StaticMeshComponent"), *InActor->GetName());
    }
};
