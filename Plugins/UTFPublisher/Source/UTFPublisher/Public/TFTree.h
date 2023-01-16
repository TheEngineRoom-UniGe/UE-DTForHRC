// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once

#include "UTFPublisher.h" // CoreMinimal, TFLog
#include "TFNode.h"
#include "Tags.h"
#include "tf2_msgs/TFMessage.h"
#include "TFTree.generated.h"

/**
* FTFTree - TF Tree
*/
USTRUCT()
struct UTFPUBLISHER_API FTFTree
{
	GENERATED_BODY()

	// Array of all nodes in the tree (used for convenient iteration)
	TArray<UTFNode*> TFNodes;

	// Destructor
	~FTFTree()
	{
		Empty();
	}

	// Init with a blank root (root will be ignored in the array)
	void Init(UTFNode* InRootNode)
	{
		Root = InRootNode;
		// If root is not blank, add to nodes array
		if (!InRootNode->IsBlank())
		{
			TFNodes.Emplace(Root);
		}
	}

	// Build tree from world
	bool Build(UWorld* InWorld)
	{
		if (Root == nullptr)
		{
			// Tree is not initialized
			return false;
		}

		// Get all objects with TF tags
		auto ObjToTagData = FTags::GetObjectKeyValuePairsMap(InWorld, TEXT("TF"));

		// Add initial root child nodes (nodes with no parent frame id, or the parent frame id equals the one from the root node)
		AddRootChildNodes(&ObjToTagData);

		// Search and add child nodes (O n^2 complexity)
		AddChildNodes(&ObjToTagData);

		// Add orphan nodes as new root trees
		AddOrphanNodes(&ObjToTagData);

		return true;
	}

	// Add node (if bAddAsOrphanIfParentNotFound is true, add node as a root child node if the parent was not found)
	bool AddNode(const FString& InChildFrameId, UObject* InAttachedObject, const FString& InParentFrameId, bool bAddAsOrphanIfParentNotFound = false)
	{
		// Check if parent in the tree
		if (UTFNode* FoundNode = FindNode(InParentFrameId))
		{
			// Create new node, and attach it to object (lifetime bound to the object now)
			UTFNode* NewTFNode = NewObject<UTFNode>(InAttachedObject);
			NewTFNode->RegisterComponent();
			NewTFNode->Init(InChildFrameId, this, InAttachedObject);
			FoundNode->AddChild(NewTFNode);
			NewTFNode->BindTransformFunction();

			// Add to array
			TFNodes.Emplace(NewTFNode);

			// Node added
			return true;
		}
		else if (bAddAsOrphanIfParentNotFound)
		{
			// Add orphan node as a root child
			return AddRootChildNode(InChildFrameId, InAttachedObject);
		}
		return false;
	}

	// Find node
	UTFNode* FindNode(const FString& InFrameId)
	{
		if (Root == nullptr)
		{
			return nullptr; // Tree is empty
		}

		if (Root->GetFrameId().Equals(InFrameId))
		{
			return Root;
		}
		else
		{
			TArray<UTFNode*> Stack;
			Stack.Push(Root);

			while (Stack.Num() > 0)
			{
				UTFNode* CurrNode = Stack.Pop();
				if (CurrNode->GetFrameId().Equals(InFrameId))
				{
					return CurrNode;
				}
				for (const auto& ChildItr : CurrNode->GetChildren())
				{
					Stack.Push(ChildItr);
				}
			}
			return nullptr; // Node not found
		}
	}

	// Add root child node (add child node directly to the root)
	bool AddRootChildNode(const FString& InChildFrameId, UObject* InAttachedObject)
	{
		if (Root)
		{
			// Create new node, and attach it to object (lifetime bound to the object now)
			UTFNode* NewTFNode = NewObject<UTFNode>(InAttachedObject);
			NewTFNode->RegisterComponent();
			NewTFNode->Init(InChildFrameId, this, InAttachedObject);
			Root->AddChild(NewTFNode);
			NewTFNode->BindTransformFunction();

			// Add to array
			TFNodes.Emplace(NewTFNode);

			// Root child node added
			return true;
		}
		return false; // Tree not initialized
	}

	// Remove node
	void RemoveNode(UTFNode* InNode)
	{
		if (InNode->IsRoot())
		{
			// Empty tree
			Empty();
		}
		else
		{
			// Remove linking to parent, link children to parent
			InNode->Clear();
			// Remove node from tree array
			TFNodes.Remove(InNode);
		}
	}

	// Get tf message
	TSharedPtr<tf2_msgs::TFMessage> GetTFMessageMsg(const FROSTime& InTime, const uint32 InSeq = 0)
	{
		// Create TFMessage
		TSharedPtr<tf2_msgs::TFMessage> TFMsgPtr =
			MakeShareable(new tf2_msgs::TFMessage());
		for (const auto& NodeItr : TFNodes)
		{
			TFMsgPtr->AddTransform(NodeItr->GetTransformStampedMsg(InTime, InSeq));
		}
		return TFMsgPtr;
	}

private:
	//  Add root child nodes (nodes that have no parent frame id, or the parent frame id equals to the root frame id)
	void AddRootChildNodes(TMap<UObject*, TMap<FString, FString>>* ObjectsToTagData)
	{
		// Iterate map and add initial root nodes
		for (auto MapItr(ObjectsToTagData->CreateIterator()); MapItr; ++MapItr)
		{
			// Set default values to child frame
			FString ChildFrameId = MapItr->Key->GetName();
			// Set child frame id from tag
			if (MapItr->Value.Contains(TEXT("ChildFrameId")))
			{
				ChildFrameId = MapItr->Value["ChildFrameId"];
			}

			// Check for parent frame id
			if (MapItr->Value.Contains(TEXT("ParentFrameId")))
			{
				// If parent frame id equals the root node frame
				if (MapItr->Value["ParentFrameId"].Equals(Root->GetFrameId()))
				{
					// Add as root child node
					AddRootChildNode(ChildFrameId, MapItr->Key);
					// Remove element from map
					MapItr.RemoveCurrent();
				}
			}
			else
			{
				// Missing parent frame id default as root child node
				AddRootChildNode(ChildFrameId, MapItr->Key);
				// Remove element from map
				MapItr.RemoveCurrent();
			}
		}
	}

	// Search and add child nodes (O n^2 complexity)
	void AddChildNodes(TMap<UObject*, TMap<FString, FString>>* ObjectsToTagData)
	{
		// Try adding objects to the tree until no more objects in the map
		// and the map size has shrinked
		bool bMapShrinked = true;
		while (ObjectsToTagData->Num() > 0 && bMapShrinked)
		{
			int32 MapSize = ObjectsToTagData->Num();
			// Iterate map and try adding objects to tree
			for (auto MapItr(ObjectsToTagData->CreateIterator()); MapItr; ++MapItr)
			{
				// Add default value to the child frame id
				FString ChildFrameId = MapItr->Key->GetName();
				// If available, set child frame id from tag
				if (MapItr->Value.Contains(TEXT("ChildFrameId")))
				{
					ChildFrameId = MapItr->Value["ChildFrameId"];
				}

				// If node has a parent frame id, try adding it to the tree
				if (MapItr->Value.Contains(TEXT("ParentFrameId")))
				{
					// If parent is already in tree, add node
					if (AddNode(ChildFrameId, MapItr->Key, MapItr->Value["ParentFrameId"]))
					{
						// Remove element from map
						MapItr.RemoveCurrent();
					}
				}
			}

			// Check if the map size has shrinked
			if (MapSize == ObjectsToTagData->Num())
			{
				bMapShrinked = false;
			}
		}
	}

	// Add orphan nodes as new root trees
	void AddOrphanNodes(TMap<UObject*, TMap<FString, FString>>* ObjectsToTagData)
	{
		// Iterate map and add initial root nodes
		for (auto MapItr(ObjectsToTagData->CreateIterator()); MapItr; ++MapItr)
		{
			// Set default values to child frame
			FString ChildFrameId = MapItr->Key->GetName();
			// If available, set child frame id from tag
			if (MapItr->Value.Contains(TEXT("ChildFrameId")))
			{
				ChildFrameId = MapItr->Value["ChildFrameId"];
			}

			// Add orphan as a root child node
			AddRootChildNode(ChildFrameId, MapItr->Key);
			// Remove element from map
			MapItr.RemoveCurrent();
		}
	}


	// Empty tree
	void Empty()
	{
		for (auto TFNodeItr : TFNodes)
		{
			// Destroy node component
			TFNodeItr->DestroyComponent();
		}
		TFNodes.Empty();
	}

	// Root node
	UTFNode* Root;
};
