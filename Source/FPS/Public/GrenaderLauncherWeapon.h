// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSWeaponActor.h"
#include "GrenaderLauncherWeapon.generated.h"

class AGrenaderProjectileActor;

UCLASS()
class FPS_API AGrenaderLauncherWeapon : public AFPSWeaponActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenaderLauncherWeapon();

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	//UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AGrenaderProjectileActor> GrenaderProjectileClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Fire() override;
};
