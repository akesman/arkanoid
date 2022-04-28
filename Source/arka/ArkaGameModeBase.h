// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ball.h"
#include "Enums.h"
#include "Life.h"
#include "GameFramework/GameModeBase.h"
#include "Score.h"
#include "arkaGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ARKA_API AarkaGameModeBase : public AGameModeBase
{
private:
	const int WidthWall = 1800;
	const int HeightWall = 2000;
	const float HeightBlock = 20;
	const float WidthBlock = 1;
	
public:
	E_Status GameState;
	ALife* Life;
	AScore* Score;
	ABall* Ball;
	GENERATED_BODY()

	AarkaGameModeBase();

	virtual void BeginPlay() override;
	void SpawnCamera();
	void CreateBlocks();
	void SpawnBall();
	void SpawnScore();
	void SpawnLife();
	void StartGame();
	void NextLevel();
	void EndLife();
	void SetBall(FVector Vector);
	void SpawnEnemy();
	void SpawnEnemyLevel2();
};
