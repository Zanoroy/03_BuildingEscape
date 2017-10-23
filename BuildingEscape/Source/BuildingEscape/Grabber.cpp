// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
	/// Attached Physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();


	/// Is the physics handle component attached?
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("There is no Physics Handle Component attached to pawn: %s!"), *GetOwner()->GetName());
	}

	/// Is the Inpit Component attached to the default Pawn?
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("There input component is attached to pawn: %s!"), *GetOwner()->GetName());

		// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("There is no input component attached to pawn: %s!"), *GetOwner()->GetName());
	}

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);

	// Use ray-cast out to reach distance
	FVector LineTrackEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * PlayerReach;

	//// Draw the debug line so we can see what we are hitting...
	//DrawDebugLine(GetWorld(),
	//	PlayerViewPointLocation,
	//	LineTrackEnd,
	//	FColor().Red,
	//	false,
	//	.0f,
	//	0,
	//	10.0f);

	// Setup the query parameters
	FCollisionQueryParams TracePrameters(FName(TEXT("")), false, GetOwner());

	// Line-trace (AKA Ray-cast) out the reach distance
	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTrackEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TracePrameters
	);

	// see what we hit..
	AActor* HitActor = Hit.GetActor();
	if (HitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player grabbed: %s"), *HitActor->GetName());
	}

}

