// Fill out your copyright notice in the Description page of Project Settings.
#include "Block.h"
#include "Components/BoxComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
ABlock::ABlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<UBoxComponent>("Component");
	RootComponent = Root;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshForHeadMesh(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
	if (MeshForHeadMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshForHeadMesh.Object);
		auto PhysicalMaterialAsset = ConstructorHelpers::FObjectFinder<UObject>(TEXT(
			"PhysicalMaterial'/Game/PM_LowFric'"));

		MeshComponent->SetPhysMaterialOverride((UPhysicalMaterial*)PhysicalMaterialAsset.Object);
	}

	MeshComponent->SetupAttachment(RootComponent);
	Root->SetCollisionProfileName("BlockALl");
}

void ABlock::BeginPlay()
{
	Super::BeginPlay();
}

void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
