// Fill out your copyright notice in the Description page of Project Settings.
#include "Carriage.h"
#include "arkaGameModeBase.h"
#include "Ball.h"
#include "Enums.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ACarriage::ACarriage()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBoxComponent>("Component");
	Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Root->SetCollisionProfileName("BlockAll");
	Root->SetNotifyRigidBodyCollision(true);
	Root->SetBoxExtent(FVector(60, 60, 120));
	Root->SetAllMassScale(0);
	RootComponent = Root;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshForHeadMesh(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
	if (MeshForHeadMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshForHeadMesh.Object);
	}

	auto PhysicalMaterialAsset = ConstructorHelpers::FObjectFinder<UObject>(TEXT(
		"PhysicalMaterial'/Game/PM_LowFric'"));

	if (PhysicalMaterialAsset.Succeeded())
	{
		MeshComponent->SetPhysMaterialOverride((UPhysicalMaterial*)PhysicalMaterialAsset.Object);
	}
	MeshComponent->SetupAttachment(RootComponent);

	SetActorScale3D(FVector(1, 5, 1));
	SetActorLocation(FVector(150, 0, 0));

	static ConstructorHelpers::FClassFinder<UUserWidget> InteractWidget(TEXT("/Game/BP_PauseGame"));

	if (InteractWidget.Succeeded())
	{
		WidgetClass = InteractWidget.Class;
	}
}

void ACarriage::BeginPlay()
{
	bActiveBonusDestroy = false;
	bActiveBonusWidth = false;
	TimeDestroy = 0;
	TimeWidth = 0;

	Root->OnComponentEndOverlap.AddDynamic(this, &ACarriage::OnOverlapEnd);
	Super::BeginPlay();
}

void ACarriage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bActiveBonusDestroy && TimeDestroy < 7)
	{
		TimeDestroy = TimeDestroy + DeltaTime;
	}
	else if (bActiveBonusDestroy)
	{
		TimeDestroy = 0;
		bActiveBonusDestroy = false;
		UE_LOG(LogTemp, Warning, TEXT("activeBonus destroy disable!"));
	}

	if (bActiveBonusWidth && TimeWidth < 7)
	{
		TimeWidth = TimeWidth + DeltaTime;
	}
	else if (bActiveBonusWidth)
	{
		TimeWidth = 0;
		bActiveBonusWidth = false;
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		PlayerPawn->SetActorScale3D(FVector(1, 5, 1));

		UE_LOG(LogTemp, Warning, TEXT("activeBonus width disable!"));
	}
}

void ACarriage::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ensure(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveRight/MoveLeft", this, &ACarriage::Move);
	PlayerInputComponent->BindAction("space", IE_Pressed, this, &ACarriage::SpaceClick);
	PlayerInputComponent->BindAction("pause", IE_Pressed, this, &ACarriage::PauseClick);
}

void ACarriage::AddWidthScale(int Width)
{
	SetActorScale3D(FVector(1, 5 + Width, 1));
}

void ACarriage::Move(float Axis)
{
	FVector Vector = FVector(0, Axis * 40, 0);
	AddActorWorldOffset(Vector, true);
	GetWorld()->GetAuthGameMode<AarkaGameModeBase>()->SetBall(GetActorLocation());
}


void ACarriage::PauseClick()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);

	if (WidgetClass)
	{
		UUserWidget* WidgetInstance = Cast<UUserWidget>(CreateWidget(GetWorld(), WidgetClass));

		if (WidgetInstance)
		{
			WidgetInstance->AddToViewport();
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Pause "));
}

void ACarriage::SpaceClick()
{
	UE_LOG(LogTemp, Warning, TEXT("SpaceClick "));
	AarkaGameModeBase* GM = GetWorld()->GetAuthGameMode<AarkaGameModeBase>();
	GM->StartGame();
}


void ACarriage::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
                          FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((Other != NULL) && (Other != this) && (OtherComp != NULL))
	{
		if (Other && (Other != this) && OtherComp && Other->GetClass() == ABall::StaticClass())
		{
			((ABall*)Other)->AddSpeed();
		}
	}
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}


void ACarriage::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
                             int32 OtherBodyIndex)
{
	if ((Other != NULL) && (Other != this) && (OtherComp != NULL))
	{
		if (Other && (Other != this) && OtherComp && Other->GetClass() == ABall::StaticClass())
		{
			((ABall*)Other)->AddSpeed();
			UE_LOG(LogTemp, Warning, TEXT("SetAllPhysicsLinearVelocity "));
		}
	}
}

void ACarriage::ActiveBonus(E_BonusType Type)
{
	switch (Type)
	{
	case E_BonusType::DestroyBall:
		{
			bActiveBonusDestroy = true;
		}
		break;
	case E_BonusType::AddPoint:
		{
			GetWorld()->GetAuthGameMode<AarkaGameModeBase>()->Score->AddScore(100);
		}
		break;
	case E_BonusType::AddWidth:
		{
			APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			PlayerPawn->SetActorScale3D(FVector(1, 5 + 2, 1));
			bActiveBonusWidth = true;
		}
		break;
	}
}
