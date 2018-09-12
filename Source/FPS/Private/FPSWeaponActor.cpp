// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSWeaponActor.h"
#include "..\Public\FPSWeaponActor.h"
#include "DrawDebugHelpers.h"
#include "kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

//准星
//枪头特效、impact特效、track特效
//瞄准线射线检测、命中debugsphere
// Sets default values
AFPSWeaponActor::AFPSWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject(TEXT("MeshComponent"));
	RootComponent = MeshComp;

	MuzzlePointName = "MuzzlePoint";
	SmokeEffectTargetParmsName = "Target";
}

// Called when the game starts or when spawned
void AFPSWeaponActor::BeginPlay()
{
	Super::BeginPlay();
}

void AFPSWeaponActor::PlayFireEffect()
{
	UGameplayStatics::SpawnEmitterAttached(FireEffect, MeshComp, MuzzlePointName);
}

void AFPSWeaponActor::PlayImpactEffect(FVector TargetLocation)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, TargetLocation, FRotator::ZeroRotator, FVector::ZeroVector);
}

void AFPSWeaponActor::PlaySmokeEffect(FVector TargetLocation)
{
	FVector SpawnLocation = MeshComp->GetSocketLocation(MuzzlePointName);
	UParticleSystemComponent* ParticleSystemComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmokeEffect, SpawnLocation, FRotator::ZeroRotator, FVector::ZeroVector);
	ParticleSystemComp->SetVectorParameter(SmokeEffectTargetParmsName, TargetLocation);
}

// Called every frame
void AFPSWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSWeaponActor::Fire()
{
	AActor* MyOwner = GetOwner();

	FVector EyeLocation = MyOwner->GetActorEyesViewPoint();
	FVector ShotDirection = MyOwner->GetActorRotation().Vector();
	FVector TraceEnd = MyOwner->GetActorLocation() + ShotDirection * 1000.0f;

	ECollisionChannel COLLISION_WEAPON;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyOwner);
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;

	FVector TracerEndPoint = TraceEnd;

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
	{
		TracerEndPoint = Hit.ImpactPoint;
		PlayImpactEffect(TracerEndPoint);
	}

	DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);

	PlayFireEffect();
	PlaySmokeEffect(TracerEndPoint);
}

