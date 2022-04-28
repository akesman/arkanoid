// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/Actor.h"
#include "Bonus.generated.h"

class UBoxComponent;
UCLASS()
class ARKA_API ABonus : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABonus();
	UPROPERTY(EditAnyWhere)
	UBoxComponent* Root;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="component")
	UStaticMeshComponent* MeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void changeColor(int Type);

	UMaterialInstanceDynamic* Material;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
	                       int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	E_BonusType Type;
};
