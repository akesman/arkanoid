#include "Score.h"
#include "Components/TextRenderComponent.h"

AScore::AScore()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<UTextRenderComponent>("Component");
	RootComponent = Root;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	MeshComponent->SetCollisionProfileName("OverlapAll");
	MeshComponent->SetupAttachment(RootComponent);
}

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

void AScore::AddScore(int AddPoint)
{
	UE_LOG(LogTemp, Warning, TEXT("AddScore"));
	Score += AddPoint;
	Root->SetText(
		FString("Score : ").Append(FString::FromInt(Score)));
}
