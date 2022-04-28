// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ball.h"
#include "Life.h"
#include "GameFramework/GameModeBase.h"
#include "Score.h"
#include "Status.h"
#include "arkaGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ARKA_API AarkaGameModeBase : public AGameModeBase
{
public:
	Status gameState;
	ALife* Life;
	AScore* Score;
	ABall* Ball;
	GENERATED_BODY()

	AarkaGameModeBase();

	virtual void BeginPlay() override;
	void spawnCamera();
	void createBlocks();
	void SpawnBall();
	void SpawnScore();
	void SpawnLife();
	void StartGame();
	void nextLevel();
	void EndLife();
	void SetBall(FVector vector);
	void SpawnEnemy();
	void SpawnEnemyLevel2();
};
