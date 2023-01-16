#include "ROSCommunication/RROSCommunication.h"
#include "ROSCommunication/RROSCommunicationComponent.h"
#include "ROSBridgeBP.h"
#include "Networking.h"
#include "IPAddress.h"
DEFINE_LOG_CATEGORY_STATIC(LogRROSCommunication, Log, All);

void UROSCommunication::Connect(const TSharedPtr<FROSBridgeHandler> &InHandler)
{
  Handler = InHandler;
  if (!Handler.IsValid())
  {
    UE_LOG(LogRROSCommunication, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
  Init();
}

void UROSCommunication::Connect(const FString &WebsocketIPAddr, const int32 &WebsocketPort)
{
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(WebsocketIPAddr, WebsocketPort));
  if (Handler.IsValid())
  {
    Handler->Connect();
    UE_LOG(LogRROSCommunication, Log, TEXT("%s is connected to ROSBridge"), *GetName())
  }
  else
  {
    UE_LOG(LogRROSCommunication, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
  Init();

}

void UROSCommunication::Disconnect()
{
  if (Handler.IsValid())
  {
    Handler->Disconnect();
  }
}

UROSBridgeBP* UROSCommunication::GetSubscriber()
{
    return (UROSBridgeBP*)Subscriber.Get();
//return (UROSBridgeBP)Subscriber->
}
void UROSCommunication::AddPose2DSubscriber(const FString& Topic, const FString& MessageType)
{
    UROSBridgeBP *USub = NewObject<UROSBridgeBP>();
    USub->SetTopic(Topic);
    USub->SetType(MessageType);
    Subscriber = MakeShareable<FROSBridgeSubscriber>(USub);
    Handler->AddSubscriber(Subscriber);

}

void UROSCommunication::PushToWebSocket(const FString  &message)
{
    Handler->PushToWebSocket(message);

}