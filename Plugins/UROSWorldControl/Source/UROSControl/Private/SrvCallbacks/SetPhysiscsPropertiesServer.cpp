// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#include "SrvCallbacks/SetPhysicsPropertiesServer.h"
#include "AssetModifier.h"
#include "Tags.h"


TSharedPtr<FROSBridgeSrv::SrvRequest> FROSSetPhysicsPropertiesServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSSetPhysicsPropertiesSrv::Request> Request =
		MakeShareable(new FROSSetPhysicsPropertiesSrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSSetPhysicsPropertiesServer::Callback(
	TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSSetPhysicsPropertiesSrv::Request> ChangePhysics =
		StaticCastSharedPtr<FROSSetPhysicsPropertiesSrv::Request>(Request);

	// get Actor with given UtagID of Controller IDMap
	AActor* ActorToBeChanged = FTags::GetActorsWithKeyValuePair(World, TEXT("SemLog"), TEXT("Id"), ChangePhysics->GetId()).Pop();

	TArray<UStaticMeshComponent*> Components;
	if (ActorToBeChanged) {
		ActorToBeChanged->GetComponents<UStaticMeshComponent>(Components);

		for (auto Component : Components)
		{
			// Execute on game thread
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
			{
				FAssetModifier::ChangePhysicsProperties(
					Component,
					ChangePhysics->GetPhysicsProperties().GetMobility(),
					ChangePhysics->GetPhysicsProperties().IsSimulatePhysics(),
					ChangePhysics->GetPhysicsProperties().GetGenerateOverlapEvents(),
					ChangePhysics->GetPhysicsProperties().GetGravity(),
					ChangePhysics->GetPhysicsProperties().GetMass());
			}, TStatId(), nullptr, ENamedThreads::GameThread);

			//wait code above to complete
			FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
		}

		return MakeShareable<FROSBridgeSrv::SrvResponse>
			(new FROSSetPhysicsPropertiesSrv::Response(true));
	}
	else
	{
		return MakeShareable<FROSBridgeSrv::SrvResponse>
			(new FROSSetPhysicsPropertiesSrv::Response(false));
	}
}
