// Fill out your copyright notice in the Description page of Project Settings.
#include "Ball.h"
#include "arkaGameModeBase.h"
#include "Enums.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBoxComponent>("Component");
	RootComponent = Root;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshForHeadMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
	if (MeshForHeadMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshForHeadMesh.Object);
	}

	auto PhysicalMaterialAsset = ConstructorHelpers::FObjectFinder<UObject>(TEXT(
		"PhysicalMaterial'/Game/M_LowMaterial'"));
	if (PhysicalMaterialAsset.Succeeded())
	{
		MeshComponent->SetMaterial(0, (UMaterial*)PhysicalMaterialAsset.Object);
	}

	Root->SetSimulatePhysics(true);
	Root->SetEnableGravity(false);
	Root->SetLinearDamping(0);

	Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	Root->BodyInstance.bLockXRotation = true;
	Root->BodyInstance.bLockYRotation = true;
	Root->BodyInstance.bLockXTranslation = true;

	Root->SetBoxExtent(FVector(60, 20, 60));
	Root->SetAllMassScale(0);

	MeshComponent->SetupAttachment(RootComponent);
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
}

void ABall::Tick(float DeltaTime)
{
	if (abs(GetVelocity().Z) < 200 && GetWorld()->GetAuthGameMode<AarkaGameModeBase>()->GameState == E_Status::Playing)
	{
		int InZ = GetVelocity().Z < 0 ? -400 : 400;
		Root->SetAllPhysicsLinearVelocity(FVector(GetVelocity().X, GetVelocity().Y, InZ));
	}

	if (GetActorLocation().Z < 0)
	{
		GetWorld()->DestroyActor(this);
		GetWorld()->GetAuthGameMode<AarkaGameModeBase>()->EndLife();
	}

	Respawn();

	Super::Tick(DeltaTime);
}

void ABall::Respawn()
{
	FVector Location = GetActorLocation();
	if (Location.Y > 1900 || Location.Y < -1900 || Location.Z > 2100)
	{
		SetActorLocation(FVector(0, 0, 500));
		Root->SetAllPhysicsLinearVelocity(FVector(0, 0, 800));
		UE_LOG(LogTemp, Warning, TEXT("RESPAWN "));
	}
}

void ABall::SetBall(FVector Vector)
{
	SetActorLocation(FVector(0, Vector.Y, 500));
}

void ABall::AddSpeed()
{
	FVector Velosity = Root->GetComponentVelocity();

	if (abs(Velosity.Y) < 200)
	{
		int Y = Velosity.Y < 0 ? -1 : 1;
		Root->SetAllPhysicsLinearVelocity(FVector(0, Y * 400, (Velosity.Z / Velosity.Z) * 2000));
	}
	else
	{
		Root->SetAllPhysicsLinearVelocity(FVector(0, Velosity.Y, 2000));
	}
}

void ABall::AddStartSpeed()
{
	Root->SetAllPhysicsLinearVelocity(FVector(0, 0, 2000));
}
