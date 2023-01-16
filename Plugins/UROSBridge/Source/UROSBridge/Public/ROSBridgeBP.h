// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeSubscriber.h"
#include "ROSBridgeBP.generated.h"

/**
 * 
 */

UCLASS(Blueprintable,DisplayName = "ROSBridge")
class UROSBRIDGE_API UROSBridgeBP : public UObject, public FROSBridgeSubscriber
{
	GENERATED_BODY()

public:

	UROSBridgeBP(const FObjectInitializer&);
	UROSBridgeBP() {}
	UROSBridgeBP(const FString& InTopic, const FString& InType);
	~UROSBridgeBP();
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ROSBridge")
	mutable float X;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ROSBridge")
	mutable float Y;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ROSBridge")
	mutable float Theta;
	virtual TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;
	
	virtual void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

};
