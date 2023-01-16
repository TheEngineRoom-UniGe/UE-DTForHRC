// Copyright 2020, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann


#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeHandler.h"
#include "CheckpointEventBroadcaster.h"
#include "world_control_msgs/srv/Checkpoint.h"


/**
 *
 */
class  FCheckpointServiceServer : public FROSBridgeSrvServer
{
public:
	FCheckpointServiceServer();
  FCheckpointServiceServer(FString Namespace, FString Name, UCheckpointEventBroadcaster* InCheckpointEventBroadcaster):
    FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/Checkpoint"))
        {
          CheckpointEventBroadcaster = InCheckpointEventBroadcaster;

        };
        ~FCheckpointServiceServer();

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
private:

  UCheckpointEventBroadcaster* CheckpointEventBroadcaster;

};
