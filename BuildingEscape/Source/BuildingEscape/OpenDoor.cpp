// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// find the owning actor
	Owner = GetOwner();

	// Get the objects name to dispaly in the string...
	FString objectName = Owner->GetName();
	FString objectPos = Owner->GetActorLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s is at pos %s!\n"), *objectName, *objectPos);
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	try {
		// Is the actor over within the Trigger?
		if (GetTotalMassOnPlate() > 50)
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
	catch (...)
	{
		UE_LOG(LogTemp, Error, TEXT("Tick threw an exception of some sort\n"));
	}
}

float UOpenDoor::GetTotalMassOnPlate()
{
	TArray<AActor*> OverloadingActors;
	PressurePlate->GetOverlappingActors(OUT OverloadingActors);
	float TotalMass = 0;
	
	for (const auto* actor : OverloadingActors)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is on plate\n"), *actor->GetName());
		
		// UPrimitiveComponent* primComponent = actor->FindComponentByClass<UPrimitiveComponent>();
		//if (primComponent)
		//{
		//	primComponent->GetMass();
		//}
		TotalMass += 0;
	}


	return TotalMass;
}

void UOpenDoor::OpenDoor(bool Open)
{
	try
	{
		// Create a FRotator	
		FRotator objectRotator = FRotator(0.0f, 0.0f, 0.0f);

		// Are we opening the door?
		if (Open)
		{
			if(DoorCurrentAngle < OpenAngle)
				DoorCurrentAngle++;
		}
		else
		{
			if (DoorCurrentAngle > 0)
				DoorCurrentAngle--;
		}
		objectRotator = FRotator(0.0f, DoorCurrentAngle, 0.0f);

		// set the rotation
		if(Owner != nullptr)
			Owner->SetActorRotation(objectRotator, ETeleportType::None);
	}
	catch(...)
	{
		UE_LOG(LogTemp, Error, TEXT("Open Door threw an exception of some sort\n"));
	}
}

