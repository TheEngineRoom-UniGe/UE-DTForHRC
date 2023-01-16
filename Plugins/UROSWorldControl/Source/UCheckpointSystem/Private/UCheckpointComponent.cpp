// Fill out your copyright notice in the Description page of Project Settings.


#include "UCheckpointComponent.h"


// Sets default values for this component's properties
UCheckpointComponent::UCheckpointComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = false;

  FString Name = GetName();
  // CheckpointEventBroadcaster = NewObject<UCheckpointEventBroadcaster>(this, FName(*(Name + TEXT("_EventBroadcaster"))));
  // CheckpointEventBroadcaster = CreateDefaultSuboject<UCheckpointEventBroadcaster>(this, FName(*(Name + TEXT("_EventBroadcaster"))));

}


// Called when the game starts
void UCheckpointComponent::BeginPlay()
{
  Super::BeginPlay();

  UE_LOG(LogTemp, Error, TEXT("%s BeginPlay"), *GetName());
  FString Name = GetName();
  // CheckpointEventBroadcaster = NewObject<UCheckpointEventBroadcaster>(this, FName(*(Name + TEXT("_EventBroadcaster"))));

  // Binds the OnGetCheckpointEvent to the ResetCheckpoint function.


  UCheckpointGameInstance* GameInstance = Cast<UCheckpointGameInstance>(UGameplayStatics::GetGameInstance(this));
  CheckpointEventBroadcaster = GameInstance->CheckpointEventBroadcaster;

  if(CheckpointEventBroadcaster)
    {
      CheckpointEventBroadcaster->OnGetCheckpointEvent.AddDynamic(this, &UCheckpointComponent::ResetCheckpoint);
      // Binds the OnSetCheckpointEvent to the SaveCheckpoint function.
      CheckpointEventBroadcaster->OnSetCheckpointEvent.AddDynamic(this, &UCheckpointComponent::SaveCheckpoint);
    }
  if(CheckpointHandler)
  {
      // Initialize the CheckpointHandler
      CheckpointHandler->Init();
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("CheckpointComponent: CheckpointHandler not initialized"));
    }
}


// Called every frame
void UCheckpointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // ...
}

void UCheckpointComponent::SaveCheckpoint(const FString& InArgs)
{
  UE_LOG(LogTemp, Error, TEXT("CheckpointComponent Save"));
  if(CheckpointHandler)
    {
      CheckpointHandler->SaveCheckpoint(CheckpointObject);
    }
}

void UCheckpointComponent::ResetCheckpoint(const FString& InArgs)
{
  UE_LOG(LogTemp, Error, TEXT("CheckpointComponent Reset"));
  if(CheckpointHandler)
    {
      CheckpointHandler->ResetCheckpoint(CheckpointObject);
    }
}
