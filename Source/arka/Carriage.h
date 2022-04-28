// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ball.h"
#include "GameFramework/Pawn.h"
#include "Carriage.generated.h"

class UBoxComponent;
UCLASS()
class ARKA_API ACarriage : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACarriage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void addWidthScale(int width);
	void MoveRight(float axis);
	void MoveLeft(float axis);
	void PauseClick();
	void SpaceClick();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="component")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnyWhere)
	UBoxComponent* Root;


	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
	                       FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
	                       const FHitResult& Hit) override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                       int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);

	TSubclassOf<UUserWidget> WidgetClass;
	UUserWidget* WidgetInstance;
};
