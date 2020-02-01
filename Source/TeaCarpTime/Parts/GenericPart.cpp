// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericPart.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AGenericPart::AGenericPart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	PartMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Part Mesh"));
	PartMeshComponent->SetupAttachment(this->RootComponent);
}

// Called when a value changes pre-game
void AGenericPart::OnConstruction(const FTransform& Transform)
{
	if (PartMesh) PartMeshComponent->SetStaticMesh(PartMesh);
	PartMeshComponent->BodyInstance.bLockRotation = SpinsWhileHeld;
}

