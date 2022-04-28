// Copyright Epic Games, Inc. All Rights Reserved.


#include "arkaGameModeBase.h"

#include "Ball.h"
#include "Block.h"
#include "Carriage.h"
#include "Enemy.h"
#include "Life.h"
#include "Score.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"


AarkaGameModeBase::AarkaGameModeBase()
{
	DefaultPawnClass = ACarriage::StaticClass();
}

void AarkaGameModeBase::BeginPlay()
{
	gameState = Status::StartGame;

	spawnCamera();
	createBlocks();
	SpawnBall();
	SpawnScore();
	SpawnLife();
	SpawnEnemy();
	
	Super::BeginPlay();
}

void AarkaGameModeBase::spawnCamera()
{
	FRotator SpawnRotation(0, 0, 0);
	FVector SpawnLoc(-2300, 0, 1200);
	ACameraActor* PlayerCameraActor = GetWorld()->SpawnActor<ACameraActor>(
		ACameraActor::StaticClass(), SpawnLoc, SpawnRotation, FActorSpawnParameters());
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (PlayerController)
	{
		PlayerCameraActor->SetOwner(PlayerController);
		PlayerController->SetViewTarget(PlayerCameraActor);
	}
}

void AarkaGameModeBase::createBlocks()
{
	FTransform SpawnTransformRight = FTransform(FRotator(), FVector(0, 1800, 0), FVector(0.1, 1, 20));

	GetWorld()->SpawnActor<ABlock>(
		ABlock::StaticClass(), SpawnTransformRight, FActorSpawnParameters());


	FTransform SpawnTransformLeft = FTransform(FRotator(), FVector(0, -1800, 0), FVector(0.1, 1, 20));

	GetWorld()->SpawnActor<ABlock>(
		ABlock::StaticClass(), SpawnTransformLeft, FActorSpawnParameters());

	FTransform SpawnTransformTop = FTransform(FRotator(), FVector(0, 0, 2000), FVector(0.1, 37, 1));

	GetWorld()->SpawnActor<ABlock>(
		ABlock::StaticClass(), SpawnTransformTop, FActorSpawnParameters());
}

void AarkaGameModeBase::SpawnBall()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	FVector location = PlayerPawn->GetActorLocation();

	FTransform SpawnTransform = FTransform(FRotator(), FVector(location.X, location.Y, 500), FVector(1, 1, 1));
	SpawnTransform.SetRotation(FQuat(0, 90, 0, 0));
	Ball = GetWorld()->SpawnActor<ABall>(
		ABall::StaticClass(), SpawnTransform, FActorSpawnParameters());
}

void AarkaGameModeBase::SpawnScore()
{
	FTransform SpawnTransform = FTransform(FRotator(), FVector(0, -2100, 2100), FVector(1, 10, 10));
	SpawnTransform.SetRotation(FQuat(0, 00, 90, 0));
	Score = GetWorld()->SpawnActor<AScore>(
		AScore::StaticClass(), SpawnTransform, FActorSpawnParameters());
}

void AarkaGameModeBase::SpawnLife()
{
	FTransform SpawnTransform = FTransform(FRotator(), FVector(0, 1000, 2100), FVector(1, 10, 10));
	SpawnTransform.SetRotation(FQuat(0, 00, 90, 0));
	Life = GetWorld()->SpawnActor<ALife>(
		ALife::StaticClass(), SpawnTransform, FActorSpawnParameters());
}


void AarkaGameModeBase::StartGame()
{
	if (gameState == Status::StartGame || gameState == Status::LoseLife)
	{
		Ball->AddStartSpeed();
	}
	gameState = Status::Playing;
}

void AarkaGameModeBase::nextLevel()
{
	gameState = Status::StartGame;

	Ball->Destroy();

	SpawnEnemyLevel2();

	SpawnBall();
}

void AarkaGameModeBase::EndLife()
{
	Life->AddLife(-1);
	if (gameState == Status::Playing && Life->life > 0)
	{
		gameState = Status::LoseLife;
		SpawnBall();
	}
	else
	{
		gameState = Status::GameOver;
	}
}

void AarkaGameModeBase::SetBall(FVector vector)
{
	if (gameState == Status::StartGame || gameState == Status::LoseLife)
		Ball->SetBall(vector);
}

void AarkaGameModeBase::SpawnEnemy()
{
	for (int i = 0; i < 8; i++) //line
	{
		for (int j = 0; j < 10; j++)
		{
			FTransform SpawnTransform = FTransform(FRotator(), FVector(0, -1525 + 340 * j, 1900 - 80 * i),
			                                       FVector(0.1, 3, 0.5));

			SpawnTransform.SetRotation(FQuat(0, 90, 0, 0));

			AEnemy* enemy = GetWorld()->SpawnActor<AEnemy>(
				AEnemy::StaticClass(), SpawnTransform, FActorSpawnParameters());
			enemy->initLevel(1);
		}
	}
}

void AarkaGameModeBase::SpawnEnemyLevel2()
{
	for (int i = 0; i < 8; i++) //line
	{
		for (int j = 0; j < 10; j++)
		{
			FTransform SpawnTransform = FTransform(FRotator(), FVector(0, -1525 + 340 * j, 1900 - 80 * i),
			                                       FVector(0.1, 3, 0.5));

			SpawnTransform.SetRotation(FQuat(0, 90, 0, 0));

			AEnemy* enemy = GetWorld()->SpawnActor<AEnemy>(
				AEnemy::StaticClass(), SpawnTransform, FActorSpawnParameters());
			enemy->initLevel(i > 4 ? 1 : 3);
		}
	}
}
