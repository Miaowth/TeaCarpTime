// Fill out your copyright notice in the Description page of Project Settings.


#include "Car.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"

// Sets default values
ACar::ACar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	CarMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Car Mesh Component"));
	CarMeshComponent->SetupAttachment(this->RootComponent);
}

// Called every frame
void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Started)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, FString("Vroom"));
	}

}

void ACar::OnConstruction(const FTransform& Transform)
{
	if (CarMesh) CarMeshComponent->SetStaticMesh(CarMesh);
}

// Called when the player restarts the car
void ACar::StartCar()
{
	Started = true;

	for (auto Stronkifier : Parts)
	{
		switch (Stronkifier.RoleOfPart)
		{
		case ERoles::ENGINE: StrengthRequirements.Engine -= Stronkifier.Part->HubDetails.Strength; break;
		case ERoles::PEDALS: StrengthRequirements.Pedals -= Stronkifier.Part->HubDetails.Strength; break;
		case ERoles::STEERINGWHEEL: StrengthRequirements.SteeringWheel -= Stronkifier.Part->HubDetails.Strength; break;
		case ERoles::WINDSCREEN: StrengthRequirements.Windscreen -= Stronkifier.Part->HubDetails.Strength; break;
		case ERoles::WHEELS: StrengthRequirements.Wheels -= Stronkifier.Part->HubDetails.Strength; break;
		case ERoles::BODY: StrengthRequirements.Body -= Stronkifier.Part->HubDetails.Strength; break;
		default: break;
		}
	}
}

