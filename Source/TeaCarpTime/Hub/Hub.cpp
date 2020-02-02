// Fill out your copyright notice in the Description page of Project Settings.

#define GETENUMSTRING(etype, evalue) ( (FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true) != nullptr) ? FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true)->GetEnumName((int32)evalue) : FString("Invalid - are you sure enum uses UENUM() macro?") )

#include "Hub.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AHub::AHub()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DepositBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Building Box"));
	DepositBox->SetupAttachment(this->RootComponent);
	DropBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Drop Box"));
	DropBox->SetupAttachment(this->RootComponent);
}

void AHub::OnConstruction(const FTransform& Transform)
{
	if (!HubZone)
	{
		const FVector SpawnLoc = GetActorLocation();
		const FRotator SpawnRot = GetActorRotation();
		FActorSpawnParameters SpawnParams;
		HubZone = GetWorld()->SpawnActor<AFence>(AFence::StaticClass(), SpawnLoc, SpawnRot, SpawnParams);
		HubZone->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}else
	{
		HubZone->SetActorLocation(GetActorLocation());
		DepositBox->SetBoxExtent(HubZone->Box->GetUnscaledBoxExtent() + DropOffDistance);
		DepositBox->SetRelativeLocation(DepositBox->GetScaledBoxExtent() - DropOffDistance);
		DropBox->SetBoxExtent(HubZone->Box->GetUnscaledBoxExtent() - 100);
		DropBox->SetRelativeLocation(DropBox->GetScaledBoxExtent() + 100);
	}
}

void AHub::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Player = Cast<ACharacter>(OtherActor);
	if (Player) PlayerWithinDropOffDistance = true;
}

void AHub::NotifyActorEndOverlap(AActor* OtherActor)
{
	Player = Cast<ACharacter>(OtherActor);
	if (Player) PlayerWithinDropOffDistance = false;
}

// Called every frame
void AHub::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AHub::AddPart(AGenericPart* PartToAdd)
{
	if (PlayerWithinDropOffDistance)
	{
		bool AlreadyHadPart = false;
		for (int32 PartCycler = 0; PartCycler < ListOfCollectedParts.Num(); PartCycler++)
		{
			if (ListOfCollectedParts[PartCycler] == PartToAdd)
			{
				++ListOfCollectedParts[PartCycler];
				AlreadyHadPart = true;
				break;
			}
		}

		if (!AlreadyHadPart)
		{
			FPartList AddingPart;
			AddingPart.Strength = PartToAdd->HubDetails.Strength;
			AddingPart.Part = PartToAdd;
			AddingPart.Quantity = 1;
			ListOfCollectedParts.Add(AddingPart);
		}
		FVector DropPoint = UKismetMathLibrary::RandomPointInBoundingBox(DropBox->GetComponentLocation(), DropBox->GetScaledBoxExtent());
		DropPoint.Z += 100;
		PartToAdd->SetActorLocation(DropPoint, false, nullptr, ETeleportType::ResetPhysics);
		PartToAdd->Collected = true;
		
		for (int32 PartIndexer = 0; PartIndexer < ListOfCollectedParts.Num(); PartIndexer++)
		{
			if (ListOfCollectedParts[PartIndexer].Part->PartName == PartToAdd->PartName)
			{
				ReduceRequirements(PartToAdd, PartIndexer);
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, PartToAdd->PartName.ToString());
				break;
			}
		}
		if (ArePartRequirementsMet())
			OnPartRequirementsMet.Broadcast();

		return true;
	}
	
	return false;
}

bool AHub::ReduceRequirements(AGenericPart* PartToReduceBy, int32 PartIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, *GETENUMSTRING("ERoles", ListOfCollectedParts[PartIndex].RoleOfPart));
	
	switch(ListOfCollectedParts[PartIndex].RoleOfPart)
	{
	case ERoles::ENGINE: Car->PartRequirements.Engine -= PartToReduceBy->HubDetails.Slots; GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Engine"); break;
	case ERoles::PEDALS: Car->PartRequirements.Pedals -= PartToReduceBy->HubDetails.Slots; GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Pedals"); break;
	case ERoles::STEERINGWHEEL: Car->PartRequirements.SteeringWheel -= PartToReduceBy->HubDetails.Slots; GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "SteeringWheel"); break;
	case ERoles::WINDSCREEN: Car->PartRequirements.Windscreen -= PartToReduceBy->HubDetails.Slots; GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Windscreen"); break;
	case ERoles::WHEELS: Car->PartRequirements.Wheels -= PartToReduceBy->HubDetails.Slots; GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Wheels"); break;
	case ERoles::MAX: Car->PartRequirements.Body -= PartToReduceBy->HubDetails.Slots; GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Body"); break;
	}

	return true;
}

bool AHub::ArePartRequirementsMet()
{
	if (Car->PartRequirements.Engine > 0 ||
		Car->PartRequirements.Pedals > 0 ||
		Car->PartRequirements.SteeringWheel > 0 ||
		Car->PartRequirements.Windscreen > 0 ||
		Car->PartRequirements.Wheels > 0 ||
		Car->PartRequirements.Body > 0)
		return false;
	return true;
}
