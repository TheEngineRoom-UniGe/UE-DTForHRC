// Copyright 2018, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "CoreMinimal.h"
#include "Json.h"
#include "ROSBridgeMsg.h"

class UROSBRIDGE_API FROSBridgeSubscriber 
{
protected:
	FString Topic;
	FString Type;

public:

	FROSBridgeSubscriber(FString InTopic, FString InType):
		Topic(InTopic), Type(InType)
	{
	}

	FROSBridgeSubscriber(){}
	virtual ~FROSBridgeSubscriber() 
	{
	}

	virtual FString GetType() const 
	{
		return Type;
	}

	virtual void SetType(FString type)
	{
		 Type=type;
	}

	virtual void SetTopic(FString topic)
	{
		Topic = topic;
	}

	virtual FString GetTopic() const 
	{
		return Topic;
	}

	virtual TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const = 0;

	virtual void Callback(TSharedPtr<FROSBridgeMsg> Msg) = 0;
	
};
