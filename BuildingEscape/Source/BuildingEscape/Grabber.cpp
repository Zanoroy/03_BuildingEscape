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

const FHitResult UGrabber::GetFirstPhysicsbodyWithinReach()
{
	// Try and grab an physicbody component within reach
	// Get player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	// Use ray-cast out to reach distance
	FVector LineTrackEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * PlayerReach;

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

	return Hit;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// If the physics handle is attached then we should
	// Move the object
	if (PhysicsHandle->GrabbedComponent)
	{
		// Get player view point
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;

		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			OUT PlayerViewPointLocation,
			OUT PlayerViewPointRotation
		);

		// Use ray-cast out to reach distance
		FVector LineTrackEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * PlayerReach;

		PhysicsHandle->SetTargetLocation(LineTrackEnd);
	}

}

/// Attempt to grab a physicbody component within "Reach"
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"));

	// see what we hit..
	FHitResult Hit = GetFirstPhysicsbodyWithinReach();
	AActor* HitActor = Hit.GetActor();

	if (HitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player grabbed: %s"), *HitActor->GetName());
		GetFirstPhysicsbodyWithinReach();

		// TODO Attach Physics handle
		PhysicsHandle->GrabComponent(
			Hit.GetComponent(),
			NAME_None,
			Hit.Actor->GetActorLocation(),
			true // Allow rotation
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"));
	PhysicsHandle->ReleaseComponent();

}

