// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Car/Car.h"
#include "Components/BoxComponent.h"
#include "Hub.generated.h"

UCLASS()
class TEACARPTIME_API AHub : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHub();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
		UBoxComponent* BuildingZone;

	UPROPERTY()
		ACar* Car;
	UPROPERTY()
		FPartList ListOfCollectedParts;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
