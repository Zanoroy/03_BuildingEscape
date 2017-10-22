// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// find the owning actor
	AActor *Owner = GetOwner();
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

	// find the owning actor
	AActor *Owner = GetOwner();

	// Get the objects name to dispaly in the string...
	FString objectName = Owner->GetName();
	FString objectPos = Owner->GetActorLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s is at pos %s!\n"), *objectName, *objectPos);
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Is the actor over within the Trigger?
	if (PressurePlate && ActorThatOpens && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		// IS the Pawn on the pressure plate?
		OpenDoor(true);
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	} 

	// Check if the door should be closed?
	if (LastDoorOpenTime + DoorCloseDelay < GetWorld()->GetTimeSeconds())
	{
		OpenDoor(false);
	}

}

void UOpenDoor::OpenDoor(bool Open)
{
	// Create a FRotator	
	FRotator objectRotator = FRotator(0.0f, 0.0f, 0.0f);

	// Are we opening the door?
	if (Open)
	{
		// DoorCurrentAngle++;
		objectRotator = FRotator(0.0f, OpenAngle, 0.0f);
	}

	// set the rotation
	Owner->SetActorRotation(objectRotator, ETeleportType::None);

}

