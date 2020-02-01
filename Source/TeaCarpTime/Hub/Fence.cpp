// Fill out your copyright notice in the Description page of Project Settings.

#include "Fence.h"


// Sets default values
AFence::AFence()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	this->PostISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PostISM"));
	this->BarISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BarISM"));
	this->Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	PostISM->SetupAttachment(this->RootComponent);
	BarISM->SetupAttachment(this->RootComponent);
	Box->SetupAttachment(this->RootComponent);

	if (this->Post) { this->PostISM->SetStaticMesh(this->Post); }
	if (this->Bar) { this->BarISM->SetStaticMesh(this->Bar); }
}

void AFence::OnConstruction(const FTransform &trans)
{
	//Sets the Post and Bar ISMs to the user-set ISMs
	if (this->Post) { this->PostISM->SetStaticMesh(this->Post); }
	if (this->Bar) { this->BarISM->SetStaticMesh(this->Bar); }

	//This runs when the player ticks the Apply Changes box
	if (ApplyChanges && this->Post && this->Bar) {

		//Clears the fence and sets the box to grow from the corner
		PostISM->ClearInstances();
		BarISM->ClearInstances();

		//Sets and moves the box to be positive if the box extent is negative
		if (Box->GetScaledBoxExtent().X < 0) {
			this->AddActorLocalOffset(FVector(Box->GetScaledBoxExtent().X * 2, 0, 0));
			this->Box->SetBoxExtent(FVector(-Box->GetScaledBoxExtent().X, Box->GetScaledBoxExtent().Y, Box->GetScaledBoxExtent().Z));
		}
		if (Box->GetScaledBoxExtent().Y < 0) {
			this->AddActorLocalOffset(FVector(0, Box->GetScaledBoxExtent().Y * 2, 0));
			this->Box->SetBoxExtent(FVector(Box->GetScaledBoxExtent().X, -Box->GetScaledBoxExtent().Y, Box->GetScaledBoxExtent().Z));
		}

		this->Box->SetRelativeLocation(FVector(Box->GetScaledBoxExtent().X, Box->GetScaledBoxExtent().Y, 0));

		//Gets the box size and sets the amount of posts and bars to spawn
		uint32 BoxLength;
		uint32 BoxWidth;
		if (Box->GetScaledBoxExtent().X >= 0) { BoxLength = 2 * FMath::CeilToInt(Box->GetScaledBoxExtent().X); }
		else { BoxLength = 2 * FMath::CeilToInt(Box->GetScaledBoxExtent().X * -1); }
		if (Box->GetScaledBoxExtent().Y >= 0) { BoxWidth = 2 * FMath::CeilToInt(Box->GetScaledBoxExtent().Y); }
		else { BoxWidth = 2 * FMath::CeilToInt(Box->GetScaledBoxExtent().Y * -1); }
		uint32 XPosts = BoxLength / FMath::CeilToInt((Post->GetBounds().GetBox().GetSize().X + Bar->GetBounds().GetBox().GetSize().X)) + 1;
		uint32 XBars = XPosts - 1;
		uint32 YPosts = BoxWidth / FMath::CeilToInt((Post->GetBounds().GetBox().GetSize().X + Bar->GetBounds().GetBox().GetSize().X)) + 1;
		uint32 YBars = YPosts - 1;
		float XLimit = 1;
		float YLimit = 1;

		//Spawns first side of the fence
		for (uint32 i = 0; i < YPosts; i++)
		{
			FTransform PostTransform;
			FVector PostSpawnLocation(0.0f, 0.0f, 0.0f);
			FRotator PostSpawnRotation(0.0f, 90.0f, 0.0f);
			PostTransform.SetRotation(PostSpawnRotation.Quaternion());
			PostSpawnLocation.Y = (Post->GetBounds().GetBox().GetSize().X + Bar->GetBounds().GetBox().GetSize().X) * i;
			PostTransform.SetLocation(PostSpawnLocation);
			PostISM->AddInstance(PostTransform);
			YLimit = PostSpawnLocation.Y;
		}
		for (uint32 i = 0; i < YBars; i++)
		{
			FTransform BarTransform;
			FVector BarSpawnLocation(0.0f, 0.0f, 0.0f);
			BarSpawnLocation.Y = ((Post->GetBounds().GetBox().GetSize().X + Bar->GetBounds().GetBox().GetSize().X) * i) + ((Bar->GetBounds().GetBox().GetSize().X + Post->GetBounds().GetBox().GetSize().X) / 2);
			FRotator BarSpawnRotation(0.0f, 90.0f, 0.0f);
			BarTransform.SetRotation(BarSpawnRotation.Quaternion());
			BarTransform.SetLocation(BarSpawnLocation);
			BarISM->AddInstance(BarTransform);
		}

		if (FenceStyle == FenceStyleEnum::BoxStyle && BoxLength >= ((Post->GetBounds().GetBox().GetSize().X * 2) + Bar->GetBounds().GetBox().GetSize().X) && BoxWidth >= ((Post->GetBounds().GetBox().GetSize().X * 2) + Bar->GetBounds().GetBox().GetSize().X))
		{
			//Spawns top and bottom sides of the fence
			for (uint32 i = 0; i < XPosts; i++)
			{
				FTransform PostTransform;
				FVector PostSpawnLocation(0.0f, 0.0f, 0.0f);
				PostSpawnLocation.X = (Post->GetBounds().GetBox().GetSize().X + Bar->GetBounds().GetBox().GetSize().X) * i;
				PostTransform.SetLocation(PostSpawnLocation);
				PostISM->AddInstance(PostTransform);
				XLimit = PostSpawnLocation.X;
				PostSpawnLocation.Y = YLimit;
				PostTransform.SetLocation(PostSpawnLocation);
				PostISM->AddInstance(PostTransform);
			}
			for (uint32 i = 0; i < XBars; i++)
			{
				FTransform BarTransform;
				FVector BarSpawnLocation(0.0f, 0.0f, 0.0f);
				BarSpawnLocation.X = ((Post->GetBounds().GetBox().GetSize().X + Bar->GetBounds().GetBox().GetSize().X) * i) + ((Bar->GetBounds().GetBox().GetSize().X + Post->GetBounds().GetBox().GetSize().X) / 2);
				BarTransform.SetLocation(BarSpawnLocation);
				BarISM->AddInstance(BarTransform);
				BarSpawnLocation.Y = YLimit;
				BarTransform.SetLocation(BarSpawnLocation);
				BarISM->AddInstance(BarTransform);
			}

			//Spawns opposite side of the fence
			for (uint32 i = 0; i < YPosts; i++)
			{
				FTransform PostTransform;
				FVector PostSpawnLocation(XLimit, 0.0f, 0.0f);
				FRotator PostSpawnRotation(0.0f, 90.0f, 0.0f);
				PostTransform.SetRotation(PostSpawnRotation.Quaternion());
				PostSpawnLocation.Y = (Post->GetBounds().GetBox().GetSize().X + Bar->GetBounds().GetBox().GetSize().X) * i;
				PostTransform.SetLocation(PostSpawnLocation);
				PostISM->AddInstance(PostTransform);
			}
			for (uint32 i = 0; i < YBars; i++)
			{
				FTransform BarTransform;
				FVector BarSpawnLocation(XLimit, 0.0f, 0.0f);
				BarSpawnLocation.Y = ((Post->GetBounds().GetBox().GetSize().X + Bar->GetBounds().GetBox().GetSize().X) * i) + ((Bar->GetBounds().GetBox().GetSize().X + Post->GetBounds().GetBox().GetSize().X) / 2);
				FRotator BarSpawnRotation(0.0f, 90.0f, 0.0f);
				BarTransform.SetRotation(BarSpawnRotation.Quaternion());
				BarTransform.SetLocation(BarSpawnLocation);
				BarISM->AddInstance(BarTransform);
			}

		}
		else if (FenceStyle == FenceStyleEnum::BoxStyle && !(BoxLength >= ((Post->GetBounds().GetBox().GetSize().X * 2) + Bar->GetBounds().GetBox().GetSize().X) && BoxWidth >= ((Post->GetBounds().GetBox().GetSize().X * 2) + Bar->GetBounds().GetBox().GetSize().X)))
		{	//Warns user if the box extent isnt big enough for a box style fence
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("To construct a Box style fence, the X and Y box extent of the 'Box' component must be able to fit at least two posts and one bar"));
		}

		this->Box->SetBoxExtent(FVector(XLimit / 2, YLimit / 2, Post->GetBounds().GetBox().GetSize().Z / 2));

		ApplyChanges = false;
	}

	this->Box->SetRelativeLocation(FVector(Box->GetScaledBoxExtent().X, Box->GetScaledBoxExtent().Y, 0));
}

