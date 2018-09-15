// Fill out your copyright notice in the Description page of Project Settings.

#include "GrenaderLauncherWeapon.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GrenaderProjectileActor.h"

// Sets default values
AGrenaderLauncherWeapon::AGrenaderLauncherWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGrenaderLauncherWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrenaderLauncherWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrenaderLauncherWeapon::Fire()
{
	UE_LOG(LogTemp, Log, TEXT("AGrenaderLauncherWeapon Fire..."));

	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Owner = this;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzlePointName);
	APawn* MyOwner = Cast<APawn>(GetOwner());
	FRotator ProjectileSpawnRotation = MyOwner->GetControlRotation();
	GetWorld()->SpawnActor<AGrenaderProjectileActor>(GrenaderProjectileClass, MuzzleLocation, ProjectileSpawnRotation, ActorSpawnParameters);
}

