// Fill out your copyright notice in the Description page of Project Settings.


#include "ROSBridgeBP.h"
#include "../Public/geometry_msgs/Pose2D.h"
#include "UObject/Object.h"


UROSBridgeBP::UROSBridgeBP(const FObjectInitializer&): FROSBridgeSubscriber()
{
    UObjectBaseUtility::AddToRoot();
}

UROSBridgeBP::~UROSBridgeBP()
{
}

UROSBridgeBP::UROSBridgeBP(const FString& InTopic, const FString& InType): FROSBridgeSubscriber(InTopic, InType)
{
    
}
TSharedPtr<FROSBridgeMsg> UROSBridgeBP::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
   
        TSharedPtr<geometry_msgs::Pose2D> TiagoPosition =
            MakeShareable<geometry_msgs::Pose2D>(new geometry_msgs::Pose2D());
        TiagoPosition->FromJson(JsonObject);

        X = TiagoPosition->GetX();
        Y = TiagoPosition->GetY(); 
        Theta= TiagoPosition->GetTheta();
        return StaticCastSharedPtr<FROSBridgeMsg>(TiagoPosition);
}

void UROSBridgeBP::Callback(TSharedPtr<FROSBridgeMsg> Msg) 
{
    
}  