// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BonusType.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UCLASS()
class ARKA_API ABall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABall();

protected:
	// Called when the game starts or when spawned

	virtual void BeginPlay() override;
private:
	bool activeBonusDestroy;
	bool activeBonusWidth;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetBall(FVector vector);
	BonusType typeBonus;

	UPROPERTY(EditAnyWhere)
	UBoxComponent* Root;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="component")
	UStaticMeshComponent* MeshComponent;


	void ActiveBonus(BonusType type);
	bool GetDestrouBonus();
	void AddSpeed();
	void AddStartSpeed();

	float timeDestroy;
	float timeWidth;
	
};
