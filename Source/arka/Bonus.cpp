// Fill out your copyright notice in the Description page of Project Settings.


#include "Bonus.h"

#include "Ball.h"
#include "Carriage.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABonus::ABonus()
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
			"PhysicalMaterial'/Game/LowMaterial'"));
		MeshComponent->SetMaterial(0, (UMaterial*)PhysicalMaterialAsset.Object);

		material = UMaterialInstanceDynamic::Create((UMaterial*)PhysicalMaterialAsset.Object, NULL);
		MeshComponent->SetMaterial(0, material);
	}


	MeshComponent->SetCollisionProfileName("OverlapAll");

	Root->SetLinearDamping(0);
	Root->SetAngularDamping(0);

	Root->SetBoxExtent(FVector(60, 60, 60));


	Root->BodyInstance.bLockXRotation = true;
	Root->BodyInstance.bLockYRotation = true;
	Root->BodyInstance.bLockZRotation = true;
	Root->BodyInstance.bLockXTranslation = true;
	
	MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABonus::BeginPlay()
{
	int rnd = FMath::RandRange(0, 2);

	if (rnd == 0) type = DestroyBall;
	if (rnd == 1) type = AddPoint;
	if (rnd == 2) type = AddWidth;
	
	changeColor(rnd);

	Root->OnComponentBeginOverlap.AddDynamic(this, &ABonus::OnBoxBeginOverlap);
	Super::BeginPlay();
}

// Called every frame
void ABonus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetActorLocation().Z < 0)
	{
		GetWorld()->DestroyActor(this);
	}
	
	Root->AddLocalOffset(FVector(0, 0, -2));
}

void ABonus::changeColor(int type2)
{
	switch (type2)
	{
	case 0:
		{
			material->SetVectorParameterValue("color", FLinearColor(1, 1, 1, 1));
		}
		break;
	case 1:
		{
			material->SetVectorParameterValue("color", FLinearColor(0, 0, 1, 1));
		}
		break;
	case 2:
		{
			material->SetVectorParameterValue("color", FLinearColor(1, 0, 0, 1));
		}
		break;
	}
}

void ABonus::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
                               int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((Other != NULL) && (Other != this) && (OtherComp != NULL))
	{
		if (Other && (Other != this) && OtherComp && Other->GetClass() == ACarriage::StaticClass())
		{
			UE_LOG(LogTemp, Warning, TEXT("Bonus use!"));

			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABall::StaticClass(), FoundActors);
			if (FoundActors.Num() != 0)
			{
				((ABall*)FoundActors[0])->ActiveBonus(type);
			}

			GetWorld()->DestroyActor(this);
		}
	}
}
