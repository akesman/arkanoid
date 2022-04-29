// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ball.h"
#include "Enums.h"
#include "GameFramework/Pawn.h"
#include "Carriage.generated.h"

class UBoxComponent;
UCLASS()
class ARKA_API ACarriage : public APawn
{
	GENERATED_BODY()

public:
	ACarriage();
	bool bActiveBonusDestroy;
	bool bActiveBonusWidth;
private:
	float TimeDestroy;
	float TimeWidth;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void AddWidthScale(int Width);
	void Move(float Axis);
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
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);
	void ActiveBonus(E_BonusType Type);

	TSubclassOf<UUserWidget> WidgetClass;
};
