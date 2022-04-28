#include "Life.h"

ALife::ALife()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UTextRenderComponent>("Component");
	RootComponent = Root;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	MeshComponent->SetCollisionProfileName("OverlapAll");
	MeshComponent->SetupAttachment(RootComponent);
}

void ALife::BeginPlay()
{
	ReloadLife();
	Super::BeginPlay();
}

void ALife::ReloadLife()
{
	Life = 3;
	Root->SetText(
		FString("Life : ").Append(FString::FromInt(Life)));
}

void ALife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALife::AddLife(int AddPoint)
{
	Life += AddPoint;
	Root->SetText(
		FString("Life : ").Append(FString::FromInt(Life)));
}
