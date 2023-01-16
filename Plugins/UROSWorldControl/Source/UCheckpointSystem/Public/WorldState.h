// Copyright 2020, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Engine/StaticMeshActor.h"
#include "WorldState.generated.h"

/**
 *
 */

USTRUCT()
struct FWorldStateInformation
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere)
  FTransform Pose;

  UPROPERTY(EditAnywhere)
  FVector LinearVel;

  UPROPERTY(EditAnywhere)
  FVector AngularVel;
};


UCLASS()
class UCHECKPOINTSYSTEM_API UWorldState : public USaveGame
{
	GENERATED_BODY()

 public:
          UPROPERTY(VisibleAnywhere)
          TMap<AStaticMeshActor*, FWorldStateInformation> WorldStateMap;

        void AddActor(AStaticMeshActor* InActor);
};
