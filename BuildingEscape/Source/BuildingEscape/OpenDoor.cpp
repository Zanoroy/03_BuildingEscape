// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// find the owning actor
	AActor *Owner = GetOwner();

	// Get the objects name to dispaly in the string...
	FString objectName = Owner->GetName();
	FString objectPos = Owner->GetActorLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s is at pos %s!\n"), *objectName, *objectPos);

	// Create a FRotator	
	FRotator objectRotator = FRotator(0.0f, 70.0f, 0.0f);

	// set the rotation
	Owner->SetActorRotation(objectRotator, ETeleportType::None);

	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

