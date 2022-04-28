// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "arkaGameModeBase.h"
#include "Ball.h"
#include "Bonus.h"
#include "Chaos/AABBTree.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


AEnemy::AEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBoxComponent>("Component");
	Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);


	RootComponent = Root;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshForHeadMesh(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
	if (MeshForHeadMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshForHeadMesh.Object);

		auto PhysicalMaterialAsset = ConstructorHelpers::FObjectFinder<UObject>(TEXT(
			"PhysicalMaterial'/Game/LowMaterial'"));

		if (PhysicalMaterialAsset.Succeeded())
		{
			material = UMaterialInstanceDynamic::Create((UMaterial*)PhysicalMaterialAsset.Object, NULL);
			MeshComponent->SetMaterial(0, material);
		}
	}

	Root->SetBoxExtent(FVector(55, 55, 45));
	MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Root->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnBoxBeginOverlap);
	Root->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnOverlapEnd);
	pointsByLvl = 5;
	Super::BeginPlay();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::initLevel(int lvl)
{
	lvlEnemy = lvl;
	changeColor();
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
			if (((ABall*)Other)->GetDestrouBonus())
			{
				destroyItem();
				addScore(pointsByLvl * lvlEnemy);
			}
		}
	}
}


void AEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
                          int32 OtherBodyIndex)
{
	if ((Other != NULL) && (Other != this) && (OtherComp != NULL))
	{
		if (Other && (Other != this) && OtherComp && Other->GetClass() == ABall::StaticClass())
		{
			lvlEnemy --;
			changeColor();
			addScore(1);
			if (lvlEnemy <= 0)
			{
				if (FMath::RandRange(0, 100) > 70)
				{
					spawnBonus(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z));
				}

				destroyItem();
			}
		}
	}
}

void AEnemy::destroyItem()
{
	GetWorld()->DestroyActor(this);
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundActors);
	if (FoundActors.Num() == 0)
	{
		GetWorld()->GetAuthGameMode<AarkaGameModeBase>()->nextLevel();
	}
}

void AEnemy::changeColor()
{
	switch (lvlEnemy)
	{
	case 1:
		{
			material->SetVectorParameterValue("color", WhiteBLOCK);
		}
		break;
	case 2:
		{
			material->SetVectorParameterValue("color", BlueBLOCK);
		}
		break;
	case 3:
		{
			material->SetVectorParameterValue("color", RedBLOCK);
		}
		break;
	}
}

void AEnemy::addScore(int multi)
{
	GetWorld()->GetAuthGameMode<AarkaGameModeBase>()->Score->AddScore(pointsByLvl * multi);
}

void AEnemy::spawnBonus(FVector location)
{
	FTransform SpawnTransform = FTransform(FRotator(), location, FVector(0.1, 1, 1));
	SpawnTransform.SetRotation(FQuat(0, 0, 0, 0));
	GetWorld()->SpawnActor<ABonus>(
		ABonus::StaticClass(), SpawnTransform, FActorSpawnParameters());
}
