// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once

#include "UTFPublisher.h" // CoreMinimal, LogTF
#include "GameFramework/Actor.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgePublisher.h"
#include "TFNode.h"
#include "TFTree.h"
#include "TFPublisher.generated.h"


UCLASS()
class UTFPUBLISHER_API ATFPublisher : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATFPublisher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when destroyed or game stopped
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	// virtual void BindEventHandler();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddObject(UObject* InObject);

	// ROSBridge server IP
	UPROPERTY(EditAnywhere, Category = TF)
	FString ServerIP;

	// ROSBridge server PORT
	UPROPERTY(EditAnywhere, Category = TF, meta = (ClampMin = 0, ClampMax = 65535))
	int32 ServerPORT;

	// TF root frame name (map, world etc.)
	UPROPERTY(EditAnywhere, Category = TF)
	FString TFRootFrameName;

	// Use tf root node as blank (avoid calculating relative transforms with root children)
	// Disable if you want to use the TF tree origin the transform of the TFPublisher actor
	UPROPERTY(EditAnywhere, Category = TF)
	bool bUseBlankRootNode;

	// Choose between variable (various publish rates for the frames)
	// or constant publish rates (all frames updated at the same time)
	UPROPERTY(EditAnywhere, Category = TF)
	bool bUseConstantPublishRate;

	// Delta time (s) between publishing (0 = on Tick)
	UPROPERTY(EditAnywhere, Category = TF, meta = (editcondition = "bUseConstantPublishRate", ClampMin = "0.0"))
	float ConstantPublishRate;

private:
	// Publish tf tree
	void PublishTF();

	// Build tree
	void BuildTFTree();

	// ROSBridge handler for ROS connection
	TSharedPtr<FROSBridgeHandler> ROSBridgeHandler;

	// ROSPublisher for publishing TF
	TSharedPtr<FROSBridgePublisher> TFPublisher;

	// Publisher timer handle (in case of custom publish rate)
	FTimerHandle TFPubTimer;

	// TF root node
	UTFNode* TFRootNode;

	// TF world tree
	FTFTree TFTree;

	// TF header message sequence
	uint32 Seq;
};
