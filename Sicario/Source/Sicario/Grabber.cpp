// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include"Engine/World.h"
#include"Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Runtime/Core/Public/Math/Vector.h"


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
	AttachInputComponent();
	AttachPhysicsHandle();	
}


void UGrabber::AttachInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("InputComponent not found"));
	}
}

void UGrabber::AttachPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandle not found"));
	}
}



// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//UE_LOG(LogTemp, Error, TEXT("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"));
	//UE_LOG(LogTemp, Error, TEXT("Actor Location %s Rotation %s"), *GetOwner()->GetActorLocation().ToString(), *GetOwner()->GetActorRotation().ToString());
	DrawDebugHitVector();
	if (PhysicsHandle && PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetHitLineEnd());
	}
}

const FHitResult UGrabber::GetFirstHitObject()
{
	///trace of specified object on view line first hit
	FHitResult Hit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, GetHitLineStart(), GetHitLineEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);
	return Hit;
}


void UGrabber::Grab() {
	UE_LOG(LogTemp, Error, TEXT("Grab executed"));
	if (PhysicsHandle) {
		auto Hit = GetFirstHitObject();
		auto ComponentToGrab = Hit.GetComponent();
		auto ActorHit = Hit.GetActor();
		if (ActorHit != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Actor %s Hit  by player pawn"), *ActorHit->GetName());
			PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation());
			//PhysicsHandle->GrabComponent(, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
		}
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Error, TEXT("release executed"));
	if (PhysicsHandle && PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->ReleaseComponent();
	}
}


FVector UGrabber::GetHitLineStart() {
	FVector PlayerLocation= GetOwner()->GetActorLocation();
	FVector test(1.f, 1.f, 08.f);
	return PlayerLocation;
	//return FVector::CrossProduct(PlayerLocation,test);

	//FVector PlayerLocation;
	//FRotator PlayerRotation;
	//GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);
	//return PlayerLocation;
}
FVector UGrabber::GetHitLineEnd() {

	FVector PlayerLocation = GetOwner()->GetActorLocation();
	FRotator PlayerRotation = GetOwner()->GetActorRotation();
	auto HitLineEnd = PlayerLocation + PlayerRotation.Vector()*80.f;
	FVector test(1.f, 1.f, 08.f);
	//return HitLineEnd;
	return FVector::CrossProduct(HitLineEnd, test);
//	FVector PlayerLocation;
//	FRotator PlayerRotation;
//	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);
//	auto viewVectorEnd = PlayerLocation + 1000 * PlayerRotation.Vector();
//	return viewVectorEnd;
}


void UGrabber::DrawDebugHitVector()
{
	//UE_LOG(LogTemp, Error, TEXT("Start % Stop %"),*GetHitLineStart().ToString(),* GetHitLineEnd().ToString());
	DrawDebugLine(GetWorld(), GetHitLineStart(), GetHitLineEnd(), FColor(255, 0, 0), false, 0.f, 0.f, 2.f);
}