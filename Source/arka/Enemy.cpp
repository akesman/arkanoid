// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "arkaGameModeBase.h"
#include "Ball.h"
#include "Bonus.h"
#include "Carriage.h"
#include "Chaos/AABBTree.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBoxComponent>("Component");
	Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Root->SetBoxExtent(FVector(55, 55, 45));
	RootComponent = Root;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshForHeadMesh(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
	if (MeshForHeadMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshForHeadMesh.Object);

		auto PhysicalMaterialAsset = ConstructorHelpers::FObjectFinder<UObject>(TEXT(
			"PhysicalMaterial'/Game/M_LowMaterial'"));

		if (PhysicalMaterialAsset.Succeeded())
		{
			Material = UMaterialInstanceDynamic::Create((UMaterial*)PhysicalMaterialAsset.Object, NULL);
			MeshComponent->SetMaterial(0, Material);
		}
	}

	MeshComponent->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
	Root->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnBoxBeginOverlap);
	Root->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnOverlapEnd);
	PointsByLvl = 5;
	Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::InitLevel(int Lvl)
{
	LvlEnemy = Lvl;
	ChangeColor();
}

void AEnemy::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
                       bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
                       const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}

void AEnemy::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
                               int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((Other != NULL) && (Other != this) && (OtherComp != NULL))
	{
		if (Other && (Other != this) && OtherComp && Other->GetClass() == ABall::StaticClass())
		{
			if (((ACarriage*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->bActiveBonusDestroy)
			{
				DestroyItem();
				AddScore(PointsByLvl * LvlEnemy);
			}
		}
	}
}


void AEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
                          int32 OtherBodyIndex)
{
	LvlEnemy --;
	ChangeColor();
	AddScore(1);
	if (LvlEnemy <= 0)
	{
		if (FMath::RandRange(0, 100) > 70)
		{
			SpawnBonus(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z));
		}

		DestroyItem();
	}
}

void AEnemy::DestroyItem()
{
	GetWorld()->DestroyActor(this);
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundActors);
	if (FoundActors.Num() == 0)
	{
		GetWorld()->GetAuthGameMode<AarkaGameModeBase>()->NextLevel();
	}
}

void AEnemy::ChangeColor()
{
	switch (LvlEnemy)
	{
	case 1:
		{
			Material->SetVectorParameterValue("color", WhiteBLOCK);
		}
		break;
	case 2:
		{
			Material->SetVectorParameterValue("color", BlueBLOCK);
		}
		break;
	case 3:
		{
			Material->SetVectorParameterValue("color", RedBLOCK);
		}
		break;
	}
}

void AEnemy::AddScore(int Multi)
{
	GetWorld()->GetAuthGameMode<AarkaGameModeBase>()->Score->AddScore(PointsByLvl * Multi);
}

void AEnemy::SpawnBonus(FVector Location)
{
	FTransform SpawnTransform = FTransform(FRotator(), Location, FVector(0.1, 1, 1));
	SpawnTransform.SetRotation(FQuat(0, 0, 0, 0));
	GetWorld()->SpawnActor<ABonus>(
		ABonus::StaticClass(), SpawnTransform, FActorSpawnParameters());
}
