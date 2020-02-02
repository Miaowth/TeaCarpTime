// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Car/Car.h"
#include "Fence.h"
#include "Components/BoxComponent.h"
#include "Hub.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPartRequirementsMet);

UCLASS()
class TEACARPTIME_API AHub : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHub();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
		AFence* HubZone;
	UPROPERTY()
		UBoxComponent* DropBox;
	UPROPERTY()
		UBoxComponent* DepositBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
		float DropOffDistance = 50;

	UPROPERTY()
		ACharacter* Player;
	UPROPERTY()
		bool PlayerWithinDropOffDistance = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		ACar* Car;
	UPROPERTY()
		TArray<FPartList> ListOfCollectedParts;

	UPROPERTY(BlueprintAssignable)
		FOnPartRequirementsMet OnPartRequirementsMet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		bool IsReady = false;


protected:

	void OnConstruction(const FTransform& Transform) override;

	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void NotifyActorEndOverlap(AActor* OtherActor) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool ReduceRequirements(AGenericPart* PartToReduceBy, int32 PartIndex);
	
	UFUNCTION(BlueprintCallable)
	bool AddPart(AGenericPart* PartToAdd);

	bool ArePartRequirementsMet();
};