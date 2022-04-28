// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

#include "arkaGameModeBase.h"
#include "BonusType.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABall::ABall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
		"PhysicalMaterial'/Game/LowMaterial'"));
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
	activeBonusDestroy = false;
	activeBonusWidth = false;
	timeDestroy = 0;
	timeWidth = 0;
	Super::BeginPlay();
}


// Called every frame
void ABall::Tick(float DeltaTime)
{
	if (abs(GetVelocity().Z) < 200 && GetWorld()->GetAuthGameMode<AarkaGameModeBase>()->gameState == Playing)
	{
		int a = GetVelocity().Z < 0 ? -400 : 400;
		Root->SetAllPhysicsLinearVelocity(FVector(GetVelocity().X, GetVelocity().Y, a));
	}

	if (GetActorLocation().Z < 0)
	{
		GetWorld()->DestroyActor(this);
		GetWorld()->GetAuthGameMode<AarkaGameModeBase>()->EndLife();
	}


	if (activeBonusDestroy && timeDestroy < 7)
	{
		timeDestroy = timeDestroy + DeltaTime;
	}
	else if (activeBonusDestroy)
	{
		timeDestroy = 0;
		activeBonusDestroy = false;
		UE_LOG(LogTemp, Warning, TEXT("activeBonus destroy disable!"));
	}


	if (activeBonusWidth && timeWidth < 7)
	{
		timeWidth = timeWidth + DeltaTime;
	}
	else if (activeBonusWidth)
	{
		timeWidth = 0;
		activeBonusWidth = false;
		UE_LOG(LogTemp, Warning, TEXT("activeBonus width disable!"));
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		PlayerPawn->SetActorScale3D(FVector(1, 5, 1));
	}

	Super::Tick(DeltaTime);
}


void ABall::SetBall(FVector vector)
{
	SetActorLocation(FVector(0, vector.Y, 500));
}

void ABall::ActiveBonus(BonusType type)
{
	if (type == AddPoint)
	{
		GetWorld()->GetAuthGameMode<AarkaGameModeBase>()->Score->AddScore(100);
	}

	if (type == DestroyBall)
	{
		typeBonus = type;
		activeBonusDestroy = true;
	}

	if (type == AddWidth)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		PlayerPawn->SetActorScale3D(FVector(1, 5 + 2, 1));
		typeBonus = type;
		activeBonusWidth = true;
	}
}

bool ABall::GetDestrouBonus()
{
	return activeBonusDestroy;
}

void ABall::AddSpeed()
{
	FVector velosity = Root->GetComponentVelocity();

	if (abs(velosity.Y) < 200)
	{
		int a = velosity.Y < 0 ? -1 : 1;
		Root->SetAllPhysicsLinearVelocity(FVector(0, a * 400, (velosity.Z / velosity.Z) * 2000));
	}
	else
	{
		Root->SetAllPhysicsLinearVelocity(FVector(0, velosity.Y, 2000));
	}
}

void ABall::AddStartSpeed()
{
	Root->SetAllPhysicsLinearVelocity(FVector(0, 0, 2000));
}
