// Fill out your copyright notice in the Description page of Project Settings.


#include "Carriage.h"

#include "arkaGameModeBase.h"
#include "Ball.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACarriage::ACarriage()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBoxComponent>("Component");
	RootComponent = Root;
	Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	Root->SetCollisionProfileName("BlockAll");

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshForHeadMesh(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
	if (MeshForHeadMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshForHeadMesh.Object);
	}

	auto PhysicalMaterialAsset = ConstructorHelpers::FObjectFinder<UObject>(TEXT(
		"PhysicalMaterial'/Game/LowFric'"));

	if (PhysicalMaterialAsset.Succeeded())
	{
		MeshComponent->SetPhysMaterialOverride((UPhysicalMaterial*)PhysicalMaterialAsset.Object);
	}
	
	MeshComponent->SetupAttachment(RootComponent);

	Root->SetNotifyRigidBodyCollision(true);

	Root->SetAllMassScale(0);
	SetActorScale3D(FVector(1, 5, 1));
	SetActorLocation(FVector(150, 0, 0));
	Root->SetBoxExtent(FVector(60, 60, 120));


	static ConstructorHelpers::FClassFinder<UUserWidget> InteractWidget(TEXT("/Game/pauseGame"));

	if (InteractWidget.Succeeded())
	{
		WidgetClass = InteractWidget.Class;
	}
}


// Called when the game starts or when spawned
void ACarriage::BeginPlay()
{
	Root->OnComponentBeginOverlap.AddDynamic(this, &ACarriage::OnBoxBeginOverlap);
	Root->OnComponentEndOverlap.AddDynamic(this, &ACarriage::OnOverlapEnd);
	Super::BeginPlay();
}

// Called every frame
void ACarriage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACarriage::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ensure(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACarriage::MoveRight);
	PlayerInputComponent->BindAxis("MoveLeft", this, &ACarriage::MoveLeft);
	PlayerInputComponent->BindAction("space", IE_Pressed, this, &ACarriage::SpaceClick);
	PlayerInputComponent->BindAction("pause", IE_Pressed, this, &ACarriage::PauseClick);
}

void ACarriage::addWidthScale(int width)
{
	SetActorScale3D(FVector(1, 5 + width, 1));
}

void ACarriage::MoveRight(float axis)
{
	FVector vector = FVector(0, axis * 40, 0);
	AddActorWorldOffset(vector, true);
	GetWorld()->GetAuthGameMode<AarkaGameModeBase>()->SetBall(GetActorLocation());
}

void ACarriage::MoveLeft(float axis)
{
	FVector vector = FVector(0, -axis * 40, 0);
	AddActorWorldOffset(vector, true);


	GetWorld()->GetAuthGameMode<AarkaGameModeBase>()->SetBall(GetActorLocation());
}

void ACarriage::PauseClick()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);

	if (WidgetClass)
	{
		WidgetInstance = Cast<UUserWidget>(CreateWidget(GetWorld(), WidgetClass));

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

void ACarriage::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
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
