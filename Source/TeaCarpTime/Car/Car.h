// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Parts/GenericPart.h"
#include "Car.generated.h"

// Struct for what parts the car has
USTRUCT(BlueprintType)
struct FPartList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AGenericPart* Part;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Quantity;

	UPROPERTY()
		float Strength;
	
	FPartList()
	{
		Part = nullptr;
		Quantity = 0;
	}

	// Checks if the input part is the same part as the list
	bool operator==(AGenericPart* EqualPart)
	{
		return Part->PartName == EqualPart->PartName;
	}

	// Adds one to the quantity of the part list
	FPartList& operator++() // Prefix ++
	{
		Quantity++;
		return *this;
	}
};

// The requirements to meet before the car is drivable
USTRUCT(BlueprintType)
struct FPartRequirements
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Engine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Pedals;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SteeringWheel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Windscreen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Wheels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Body;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Fuel;

	FPartRequirements()
	{
		Engine = 5;
		Pedals = 3;
		SteeringWheel = 1;
		Windscreen = 2;
		Wheels = 4;
		Body = 5;
		Fuel = 10;
	}
};

// The requirements to meet before the car is drivable
USTRUCT(BlueprintType)
struct FStrengthRequirements
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Engine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Pedals;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SteeringWheel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Windscreen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Wheels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Body;

	FStrengthRequirements()
	{
		Engine = 12;
		Pedals = 5;
		SteeringWheel = 2;
		Windscreen = 5;
		Wheels = 8;
		Body = 13;
	}
};

UCLASS()
class TEACARPTIME_API ACar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACar();

	UPROPERTY()
		UStaticMeshComponent* CarMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
		UStaticMesh* CarMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
		TArray<FPartList> Parts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
		FPartRequirements PartRequirements;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
		FStrengthRequirements StrengthRequirements;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Driving")
		bool Started = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Driving")
		float Strength;

	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnConstruction(const FTransform& Transform) override;

	// Called when the player restarts the car
	UFUNCTION(BlueprintCallable)
	void StartCar();

};
