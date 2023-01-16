// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#include "TFNode.h"
#include "TFTree.h"
#include "Conversions.h"
#include "geometry_msgs/TransformStamped.h"

// Sets default values for this component's properties
UTFNode::UTFNode()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Destructor
UTFNode::~UTFNode()
{
}

// Called when the game starts
void UTFNode::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

// Called when the destroy process begins
void UTFNode::BeginDestroy()
{
	Super::BeginDestroy();
	
	// Remove itself from the TF world tree
	if (OwnerTree != nullptr)
	{
		OwnerTree->RemoveNode(this);
	}
}

// Init node with attached parent as base class UObject
void UTFNode::Init(const FString& InFrameId, FTFTree* InOwnerTree, UObject* InAttachedObject)
{
	FrameId = InFrameId;
	OwnerTree = InOwnerTree;
	GetTransformFunctionPtr = &UTFNode::GetTransform_AsIdentity;

	// Check attached actor type
	if (auto AA = Cast<AActor>(InAttachedObject))
	{
		ActorBaseObject = AA;
	}
	else if (auto USC = Cast<USceneComponent>(InAttachedObject))
	{
		SceneComponentBaseObject = USC;
	}
}

// Bind transform function pointers
void UTFNode::BindTransformFunction()
{
	if (Parent)
	{
		if (Parent->IsBlank())
		{
			// Node has parent, parent is blank, return world transform (avoid calculating unnecessary relative TFtransform)
			if (ActorBaseObject)
			{
				GetTransformFunctionPtr = &UTFNode::GetWorldTransform_FromActor;
			}
			else if (SceneComponentBaseObject)
			{
				GetTransformFunctionPtr = &UTFNode::GetWorldTransform_FromSceneComponent;
			}
		}
		else
		{
			// Node has parent, parent has a transform, calculate relative transform
			if (ActorBaseObject)
			{
				GetTransformFunctionPtr = &UTFNode::GetRelativeTransform_FromActor;
			}
			else if (SceneComponentBaseObject)
			{
				GetTransformFunctionPtr = &UTFNode::GetRelativeTransform_FromSceneComponent;
			}
		}
	}
	else 
	{
		// Node is root, not blank, return roots world transform (avoid calculating unnecessary relative TFtransform)
		if (!IsBlank())
		{
			if (ActorBaseObject)
			{
				GetTransformFunctionPtr = &UTFNode::GetWorldTransform_FromActor;
			}
			else if (SceneComponentBaseObject)
			{
				GetTransformFunctionPtr = &UTFNode::GetWorldTransform_FromSceneComponent;
			}
		}
	}
}

// Get as geometry_msgs::TransformStamped message
geometry_msgs::TransformStamped UTFNode::GetTransformStampedMsg(const FROSTime& InTime, const uint32 InSeq) const
{
	geometry_msgs::TransformStamped StampedTransformMsg;

	std_msgs::Header Header;
	Header.SetSeq(InSeq);
	const FString ParentFrameId = Parent != nullptr ?
		Parent->FrameId : TEXT("None");
	Header.SetFrameId(ParentFrameId);
	Header.SetStamp(InTime);

	// Transform to ROS coordinate system
	FTransform ROSTransf = FConversions::UToROS(GetTransform());

	geometry_msgs::Transform TransfMsg(
		geometry_msgs::Vector3(ROSTransf.GetLocation()),
		geometry_msgs::Quaternion(ROSTransf.GetRotation()));

	StampedTransformMsg.SetHeader(Header);
	StampedTransformMsg.SetChildFrameId(FrameId);
	StampedTransformMsg.SetTransform(TransfMsg);

	return StampedTransformMsg;
}

// Add child
void UTFNode::AddChild(UTFNode* InChildNode)
{
	Children.Emplace(InChildNode);
	InChildNode->Parent = this;
}

// Clear node linkings in tree, remove linking to parent, link children to parent
void UTFNode::Clear()
{
	if (!IsRoot())
	{
		// Remove yourself as a child of parent
		Parent->Children.Remove(this);
		// Link your children to parent
		for (auto& ChildItr : Children)
		{
			ChildItr->Parent = Parent;
			// Recalculate transform binding function
			// avoid unnecessary relative transform calculation
			// if the child becomes a root child of a blank node
			ChildItr->BindTransformFunction();
		}
	}
	// If the node is root, the whole tree will get deleted;
}

// Get node transform
FORCEINLINE FTransform UTFNode::GetTransform() const
{
	return (this->*GetTransformFunctionPtr)();
}

// Get the transform (default identity)
FORCEINLINE FTransform UTFNode::GetTransform_AsIdentity() const
{
	return FTransform::Identity;
}

// Get the world transform of actor (node has non blank parent)
FORCEINLINE FTransform UTFNode::GetRelativeTransform_FromActor() const
{
	return ActorBaseObject->GetTransform().GetRelativeTransform(
		Parent->ActorBaseObject->GetTransform());
}

// Get the world transform of scene component
FORCEINLINE FTransform UTFNode::GetRelativeTransform_FromSceneComponent() const
{
	return SceneComponentBaseObject->GetComponentTransform().GetRelativeTransform(
		Parent->SceneComponentBaseObject->GetComponentTransform());
}

// Get the world transform of actor
FORCEINLINE FTransform UTFNode::GetWorldTransform_FromActor() const
{
	return ActorBaseObject->GetTransform();
}

// Get the world transform of scene component
FORCEINLINE FTransform UTFNode::GetWorldTransform_FromSceneComponent() const
{
	return SceneComponentBaseObject->GetComponentTransform();
}
