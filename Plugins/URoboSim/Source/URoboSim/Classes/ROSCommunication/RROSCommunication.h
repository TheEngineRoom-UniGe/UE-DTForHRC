// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "Physics/RModel.h"
#include "ROSUtilities.h"
#include "ROSBridgeHandler.h"
#include "WebSocket.h"
// clang-format off
#include "RROSCommunication.generated.h"
// clang-format on

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API UROSCommunication : public UObject
{
  GENERATED_BODY()

public:

  UFUNCTION(BlueprintCallable, meta = (DisplayName = "Connect", Keywords = ""), Category = "ROSBridge")
  void Connect(const FString &WebsocketIPAddr, const int32 &WebsocketPort);

  void Connect(const TSharedPtr<FROSBridgeHandler> &InHandler);

  UFUNCTION(BlueprintCallable, meta = (DisplayName = "Disconnect", Keywords = ""), Category = "ROSBridge")
  void Disconnect();

  UFUNCTION(BlueprintCallable, meta = (DisplayName = "AddPosition2DSubscriber", Keywords = ""), Category = "ROSBridge")
  void AddPose2DSubscriber(const FString& Topic, const FString& MessageType);
  
  UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetSubscriber", Keywords = ""), Category = "ROSBridge")
  UROSBridgeBP* GetSubscriber();

  UFUNCTION(BlueprintCallable, meta = (DisplayName = "PushData", Keywords = ""), Category = "ROSBridge")
  void PushToWebSocket(const FString &message);



public:
  virtual void Tick() {}

protected:
  virtual void Init() {}

protected:
  TSharedPtr<FROSBridgeHandler> Handler;
  TSharedPtr<FROSBridgeSubscriber> Subscriber;
  
};
