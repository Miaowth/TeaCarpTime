// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine.h"
#include "Fence.generated.h"

UENUM()
enum class FenceStyleEnum : uint8 {
	LineStyle UMETA(DisplayName = "Line"),
	BoxStyle UMETA(DisplayName = "Box"),
};


UCLASS()
class TEACARPTIME_API AFence : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFence();

	UPROPERTY(EditAnywhere, Category = "Fence")
		bool ApplyChanges = false;

	UPROPERTY(EditAnywhere, Category = "Fence")
		FenceStyleEnum FenceStyle = FenceStyleEnum::LineStyle;

	UPROPERTY(EditAnywhere, Category = "Fence")
		UStaticMesh * Post;

	UPROPERTY(EditAnywhere, Category = "Fence")
		UStaticMesh * Bar;

	UPROPERTY(EditAnywhere, Category = "Fence")
		UBoxComponent * Box;

	UPROPERTY()
		UInstancedStaticMeshComponent * PostISM;

	UPROPERTY()
		UInstancedStaticMeshComponent * BarISM;

public:

	virtual void OnConstruction(const FTransform &trans) override;


};
