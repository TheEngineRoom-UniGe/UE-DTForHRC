// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once

#include "UTFPublisher.h" // CoreMinimal, TFLog
#include "Components/ActorComponent.h"
#include "geometry_msgs/TransformStamped.h"
#include "TFNode.generated.h"

// Forward declaration to avoid circular dependency
struct FTFTree;

/**
* UTFNode - TF Node, inherits from UActorComponent to have life duration synced
*
*  - pointer to parent node
*  - array of children nodes
*  - node data:
*    - frame name
*    - pointer to parent entity (AActor or USceneComponent) with access to FTransform, nullptr if blank node
*/
UCLASS()
class UTFPUBLISHER_API UTFNode : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTFNode();

	// Destructor
	virtual ~UTFNode();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called when the destroy process begins
	virtual void BeginDestroy() override;

public:	
	// Init node with attached parent as base class UObject
	void Init(const FString& InFrameId, FTFTree* InOwnerTree, UObject* InAttachedObject = nullptr);

	// Bind transform function pointers
	void BindTransformFunction();

	// Get frame id
	FString GetFrameId() const { return FrameId; }

	// Get children
	const TArray<UTFNode*>& GetChildren() const { return Children; }

	// Check if node is blank
	bool IsBlank() const { return ActorBaseObject == nullptr && SceneComponentBaseObject == nullptr; }

	// Check if node is root
	bool IsRoot() const { return Parent == nullptr; }

	// Get transform stamped msg
	geometry_msgs::TransformStamped GetTransformStampedMsg(const FROSTime& InTime, const uint32 InSeq = 0) const;

	// Add child
	void AddChild(UTFNode* InChildNode);

	// Clear node from tree, remove linking to parent, and link children to parent
	void Clear();

private:
	// Get tf transform
	FORCEINLINE FTransform GetTransform() const;

	// Get the transform (default identity)
	FORCEINLINE FTransform GetTransform_AsIdentity() const;

	// Get the relative transform from actor (node has non blank parent)
	FORCEINLINE FTransform GetRelativeTransform_FromActor() const;

	// Get the relative transform from scene component (node has non blank parent)
	FORCEINLINE FTransform GetRelativeTransform_FromSceneComponent() const;

	// Get the world transform of actor (node has blank or no parent)
	FORCEINLINE FTransform GetWorldTransform_FromActor() const;

	// Get the world transform of scene component (node has blank or no parent)
	FORCEINLINE FTransform GetWorldTransform_FromSceneComponent() const;

	// Get relative transform function pointer variable type
	typedef FTransform(UTFNode::*GetTransformFuncPtrType)() const;
	// Function pointer to the get the relative transform function
	GetTransformFuncPtrType GetTransformFunctionPtr;
	
	// Parent
	UTFNode* Parent;

	// Array of children nodes
	TArray<UTFNode*> Children;

	// Name of the frame id (tf equivalent of child_frame_id) 
	FString FrameId;

	// Base object type to get the FTransform (of AACtor or USceneComponent)
	AActor* ActorBaseObject;
	USceneComponent* SceneComponentBaseObject;

	// Pointer to the owner tree (to remove itself from tree in case of destruction)
	FTFTree* OwnerTree;
};
