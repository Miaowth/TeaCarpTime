// Fill out your copyright notice in the Description page of Project Settings.


#include "Hub.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AHub::AHub()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DropOffBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Building Box"));
	DropOffBox->SetupAttachment(this->RootComponent);
}

// Called when the game starts or when spawned
void AHub::BeginPlay()
{
	Super::BeginPlay();
	
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
		DropOffBox->SetBoxExtent(HubZone->Box->GetUnscaledBoxExtent() + DropOffDistance);
		this->DropOffBox->SetRelativeLocation(DropOffBox->GetScaledBoxExtent() - DropOffDistance);
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
		FVector DropPoint = UKismetMathLibrary::RandomPointInBoundingBox(HubZone->Box->GetComponentLocation(), HubZone->Box->GetScaledBoxExtent());
		DropPoint.Z += 500;
		PartToAdd->SetActorLocation(DropPoint);

		for (int32 i = 0; i < ListOfCollectedParts.Num(); i++)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, ListOfCollectedParts[i].Part->PartName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString::FromInt(ListOfCollectedParts[i].Quantity));
		}

		return true;
	}
	else { return false; }
}
