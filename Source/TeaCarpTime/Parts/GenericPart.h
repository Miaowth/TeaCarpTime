// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Actor.h"
#include "GenericPart.generated.h"

// Enum bitmask for the compatibility of the parts
UENUM(BlueprintType, Meta = (Bitflags))
enum ERoles
{
	ENGINE			= 1		UMETA(DisplayName = "Engine Compatible"),
	PEDALS			= 2		UMETA(DisplayName = "Pedal Compatible"),
	STEERINGWHEEL	= 4		UMETA(DisplayName = "Steering Wheel Compatible"),
	WINDSCREEN		= 8		UMETA(DisplayName = "Windscreen Compatible"),
	WHEELS			= 16	UMETA(DisplayName = "Wheel Compatible"),
	MAX				= 32	UMETA(DisplayName = "N/A")
};

ENUM_CLASS_FLAGS(ERoles);

// Holds details that'll be used in the building hub
USTRUCT(BlueprintType)
struct FHubDetails
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* HubPiece;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Slots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Strength;

	FHubDetails()
	{
		HubPiece = nullptr;
		Slots = 0;
		Strength = 0;
	}
};

UCLASS()
class TEACARPTIME_API AGenericPart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenericPart();

	// Appearance of the part
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
		UStaticMesh* PartMesh;
	UPROPERTY()
		UStaticMeshComponent* PartMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
		FName PartName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
		bool SpinsWhileHeld = true;

	// Utility variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility")
		USoundBase* PartSound;
	UPROPERTY()
		UAudioComponent* PartSoundComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility")
		bool IsTrash = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility")
		bool IsFuel = false;
	UPROPERTY()
		UPrimitiveComponent* PrimitiveComp;
	
	// The details used by the building hub
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hub")
		FHubDetails HubDetails;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hub", meta = (Bitmask, BitmaskEnum = "ERoles"))
		int32 PartRoles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hub")
		bool Collected = false;


	UFUNCTION(BlueprintCallable)
	void SetMesh();
	// Called when a value changes pre-game
	void OnConstruction(const FTransform& Transform) override;

};
