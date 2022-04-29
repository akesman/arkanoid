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
	ALife();

protected:
	virtual void BeginPlay() override;
	void ReloadLife();

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnyWhere)
	UTextRenderComponent* Root;

	void AddLife(int AddPoint);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="component")
	UStaticMeshComponent* MeshComponent;

	int Life;

};
