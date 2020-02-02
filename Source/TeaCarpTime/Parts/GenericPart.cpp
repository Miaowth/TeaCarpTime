// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericPart.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AGenericPart::AGenericPart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<UPrimitiveComponent>(TEXT("Root"));
	PartMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Part Mesh"));
	PartMeshComponent->SetupAttachment(this->RootComponent);
	PartSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Part Sound"));
	PartSoundComponent->SetupAttachment(this->RootComponent);
	PartMeshComponent->SetSimulatePhysics(true);
	PartMeshComponent->SetEnableGravity(true);
	PartMeshComponent->SetCollisionProfileName(FName("PhysicsActor"));
}

// Called when a value changes pre-game
void AGenericPart::OnConstruction(const FTransform& Transform)
{
	if (PartMesh) PartMeshComponent->SetStaticMesh(PartMesh);
	PartMeshComponent->BodyInstance.bLockRotation = SpinsWhileHeld;
}

void AGenericPart::SetMesh()
{
	if(PartMesh) PartMeshComponent->SetStaticMesh(PartMesh);
	if (PartSound) PartSoundComponent->SetSound(PartSound);

}