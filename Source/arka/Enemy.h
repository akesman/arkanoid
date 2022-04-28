// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Score.h"
#include "Enemy.generated.h"

class UBoxComponent;
UCLASS()
class ARKA_API AEnemy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemy();


	int lvlEnemy;
	int pointsByLvl;

	 const FLinearColor WhiteBLOCK = FLinearColor(1, 1, 1, 1);
	 const FLinearColor BlueBLOCK = FLinearColor(0, 0, 1, 1);
	 const FLinearColor RedBLOCK = FLinearColor(1, 0, 0, 1);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void initLevel(int lvl);

	UPROPERTY(EditAnyWhere)
	UBoxComponent* Root;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="component")
	UStaticMeshComponent* MeshComponent;


	UMaterialInstanceDynamic* material;

	UFUNCTION()
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
	                       FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
	                       const FHitResult& Hit) override;


	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                       int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void addScore(int multi);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void destroyItem();
	void changeColor();
	void spawnBonus(FVector location);
};
