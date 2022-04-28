// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "Life.generated.h"

UCLASS()
class ARKA_API ALife : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALife();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void reloadLife();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnyWhere)
	UTextRenderComponent* Root;

	void AddLife(int addPoint);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="component")
	UStaticMeshComponent* MeshComponent;

	int life;

};
