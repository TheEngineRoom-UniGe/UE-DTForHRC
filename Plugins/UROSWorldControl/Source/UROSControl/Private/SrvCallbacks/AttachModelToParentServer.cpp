// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#include "SrvCallbacks/AttachModelToParentServer.h"
#include "AssetModifier.h"
#include "Tags.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSAttachModelToParentServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSAttachModelToParentSrv::Request> Request_ =
		MakeShareable(new FROSAttachModelToParentSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSAttachModelToParentServer::Callback(
	TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSAttachModelToParentSrv::Request> AttachModelToParentRequest =
		StaticCastSharedPtr<FROSAttachModelToParentSrv::Request>(Request);


	TMap<FString, AActor*> Actors = FTags::GetKeyValuesToActor(World, TEXT("SemLog"), TEXT("Id"));


	AActor** Child = Actors.Find(AttachModelToParentRequest->GetChildId());
	AActor** Parent = Actors.Find(AttachModelToParentRequest->GetParentId());
	if (Child && Parent)
	{
		//Actors were found and will be attached, in GameThread
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			FAssetModifier::AttachToParent(*Parent, *Child);
		}, TStatId(), nullptr, ENamedThreads::GameThread);

		//wait code above to complete
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

		return MakeShareable<FROSBridgeSrv::SrvResponse>
			(new FROSAttachModelToParentSrv::Response(true));
	}
	else
	{
		if (!Child)
		{
			UE_LOG(LogTemp, Error, TEXT("Actor with id:\"%s\" does not exist."), *AttachModelToParentRequest->GetChildId());
		}
		if (!Parent)
		{
			UE_LOG(LogTemp, Error, TEXT("Actor with id:\"%s\" does not exist."), *AttachModelToParentRequest->GetParentId());
		}
	}
	
	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSAttachModelToParentSrv::Response(false));
}
