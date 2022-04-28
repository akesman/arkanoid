// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Score.generated.h"

class UTextRenderComponent;
class UBoxComponent;
UCLASS()
class ARKA_API AScore : public AActor
{
	GENERATED_BODY()
	
public:	
	AScore();
	
protected:
	virtual void BeginPlay() override;
	
private:
	int Score;
	
public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnyWhere)
	UTextRenderComponent* Root;

	void AddScore(int AddPoint);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="component")
	UStaticMeshComponent* MeshComponent;
};
