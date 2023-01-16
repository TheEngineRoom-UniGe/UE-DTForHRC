// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointServiceServer.h"

FCheckpointServiceServer::FCheckpointServiceServer()
{
}

FCheckpointServiceServer::~FCheckpointServiceServer()
{
}

TSharedPtr<FROSBridgeSrv::SrvRequest> FCheckpointServiceServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FCheckpointSrv::Request> Request =
		MakeShareable(new FCheckpointSrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FCheckpointServiceServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
  bool ServiceSuccess = false;
        TSharedPtr<FCheckpointSrv::Request> MoveRequest =
		StaticCastSharedPtr<FCheckpointSrv::Request>(Request);


        FString Command = MoveRequest->GetCommand();
        FString Args = MoveRequest->GetArgs();

        if(CheckpointEventBroadcaster)
          {
            FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
                {
                  CheckpointEventBroadcaster->Broadcast(Command, Args);
                }, TStatId(), nullptr, ENamedThreads::GameThread);
            FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
            ServiceSuccess = true;
          }
        else
          {
            UE_LOG(LogTemp, Error, TEXT("CheckpointEventBroadcaster of CheckpointService is nullptr"));
          }

        return MakeShareable<FROSBridgeSrv::SrvResponse>
          (new FCheckpointSrv::Response(ServiceSuccess));
}
