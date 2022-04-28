#pragma once

UENUM()
enum class E_BonusType :uint8
{
	DestroyBall=0 UMETA(DisplayName = "DestroyBall"),
	AddPoint=1 UMETA(DisplayName = "AddPoint"),
	AddWidth=2 UMETA(DisplayName = "AddWidth"),
};

UENUM()
enum class E_Status: uint8
{
	StartGame = 0 UMETA(DisplayName = "StartGame"),
	PauseGame = 1 UMETA(DisplayName = "PauseGame"),
	Playing = 2 UMETA(DisplayName = "Playing"),
	LoseLife = 3 UMETA(DisplayName = "LoseLife"),
	GameOver= 4 UMETA(DisplayName = "GameOver"),
};
