// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UCLASS()
class ARKA_API ABall : public AActor
{
	GENERATED_BODY()

public:
	ABall();

protected:

	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	void Respawn();
	void SetBall(FVector Vector);
	
	UPROPERTY(EditAnyWhere)
	UBoxComponent* Root;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="component")
	UStaticMeshComponent* MeshComponent;
	
	void AddSpeed();
	void AddStartSpeed();
};
