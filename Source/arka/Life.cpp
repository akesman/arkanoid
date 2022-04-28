// Fill out your copyright notice in the Description page of Project Settings.


#include "Life.h"

// Sets default values
ALife::ALife()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UTextRenderComponent>("Component");

	RootComponent = Root;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	/*ConstructorHelpers::FObjectFinder<UStaticMesh> MeshForHeadMesh(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
	if (MeshForHeadMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshForHeadMesh.Object);
		auto PhysicalMaterialAsset = ConstructorHelpers::FObjectFinder<UObject>(TEXT(
			"PhysicalMaterial'/Game/LowFric'"));

		MeshComponent->SetPhysMaterialOverride((UPhysicalMaterial*)PhysicalMaterialAsset.Object);
	}*/

	MeshComponent->SetCollisionProfileName("OverlapAll");
	MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALife::BeginPlay()
{
	reloadLife();
	Super::BeginPlay();
}

void ALife::reloadLife()
{
	life = 3;
	Root->SetText(
		FString("Life : ").Append(FString::FromInt(life)));
}

// Called every frame
void ALife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALife::AddLife(int addPoint)
{
	life += addPoint;
	Root->SetText(
		FString("Life : ").Append(FString::FromInt(life)));
}
