// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

/// locate the attached physics handle
void UGrabber::FindPhysicsHandleComponent()
{
	/// Attached Physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	/// Is the physics handle component attached?
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("There is no Physics Handle Component attached to pawn: %s!"), *GetOwner()->GetName());
	}
}

/// Setup the input Component (only avaliable during play)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	/// Is the Inpit Component attached to the default Pawn?
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("There input component is attached to pawn: %s!"), *GetOwner()->GetName());

		// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("There is no input component attached to pawn: %s!"), *GetOwner()->GetName());
	}
}

/// Get the players reach start point
FVector UGrabber::GetReachPointStart()
{
	// Get player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}

/// Calculate the end point of the players reach
FVector UGrabber::GetReachPointEnd()
{
	// Get player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * PlayerReach;
}

const FHitResult UGrabber::GetFirstPhysicsbodyWithinReach()
{
	// Setup the query parameters
	FCollisionQueryParams TracePrameters(FName(TEXT("")), false, GetOwner());
	// Line-trace (AKA Ray-cast) out the reach distance
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachPointStart(),
		GetReachPointEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TracePrameters
	);

	return HitResult;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// If the physics handle is attached then we should, move the object
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachPointEnd());
	}

}

/// Attempt to grab a physicbody component within "Reach"
void UGrabber::Grab()
{
	// See what we hit..
	FHitResult Hit = GetFirstPhysicsbodyWithinReach();
	AActor* HitActor = Hit.GetActor();

	/// IF we hit something attach a physic handle
	if (HitActor)
	{
		// TODO Attach Physics handle
		PhysicsHandle->GrabComponent(
			Hit.GetComponent(), // the mesh we are attaching to
			NAME_None, // No bones needed
			Hit.Actor->GetActorLocation(),
			true // Allow rotation
		);
	}
}

/// Release the grabbed component
void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}
