// Fill out your copyright notice in the Description page of Project Settings.


#include "Score.h"

#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
AScore::AScore()
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
void AScore::BeginPlay()
{
	Score = 0;
	Root->SetText(
		FString("Score : ").Append(FString::FromInt(Score)));
	Super::BeginPlay();
}

// Called every frame
void AScore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AScore::AddScore(int addPoint)
{
	UE_LOG(LogTemp, Warning, TEXT("AddScore"));
	Score += addPoint;
	Root->SetText(
		FString("Score : ").Append(FString::FromInt(Score)));
}
