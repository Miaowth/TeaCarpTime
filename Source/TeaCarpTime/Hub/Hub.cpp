// Fill out your copyright notice in the Description page of Project Settings.


#include "Hub.h"

// Sets default values
AHub::AHub()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	BuildingZone = CreateDefaultSubobject<UBoxComponent>(TEXT("Building Box"));
	BuildingZone->SetupAttachment(this->RootComponent);
}

// Called when the game starts or when spawned
void AHub::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHub::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

