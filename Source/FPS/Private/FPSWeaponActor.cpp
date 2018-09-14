// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSWeaponActor.h"
#include "DrawDebugHelpers.h"
#include "kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"

//准星
//枪头特效、impact特效、track特效
//瞄准线射线检测、命中debugsphere
// Sets default values
AFPSWeaponActor::AFPSWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
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
	UE_LOG(LogTemp, Log, TEXT("PlayFireEffect"));
	UGameplayStatics::SpawnEmitterAttached(FireEffect, MeshComp, MuzzlePointName);
}

void AFPSWeaponActor::PlayImpactEffect(FVector ImpactPoint)
{
	UE_LOG(LogTemp, Log, TEXT("PlayImpactEffect"));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, ImpactPoint, FRotator::ZeroRotator, FVector::ZeroVector);
}

void AFPSWeaponActor::PlaySmokeEffect(FVector TargetImpactPoint)
{
	UE_LOG(LogTemp, Log, TEXT("PlaySmokeEffect"));
	FVector SpawnLocation = MeshComp->GetSocketLocation(MuzzlePointName);
	UParticleSystemComponent* ParticleSystemComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmokeEffect, SpawnLocation, FRotator::ZeroRotator, FVector::ZeroVector);
	ParticleSystemComp->SetVectorParameter(SmokeEffectTargetParmsName, TargetImpactPoint);
}

// Called every frame
void AFPSWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPSWeaponActor::Fire()
{
	UE_LOG(LogTemp, Log, TEXT("AFPSWeaponActor Fire"));
	AActor* MyOwner = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotator;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotator);

	FVector ShotDirection = EyeRotator.Vector();
	FVector TraceEnd = EyeLocation + ShotDirection * 1000.0f;

	ECollisionChannel COLLISION_WEAPON = ECollisionChannel::ECC_GameTraceChannel1;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyOwner);
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;

	FVector TracerEndPoint = TraceEnd;

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
	{
		UE_LOG(LogTemp, Log, TEXT("AFPSWeaponActor Hit"));

		TracerEndPoint = Hit.ImpactPoint;
		PlayImpactEffect(TracerEndPoint);
		DrawDebugSphere(GetWorld(), TracerEndPoint, 10.0f, 16, FColor::Yellow, false, 1.0f);
	}

	DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);

	PlayFireEffect();
	PlaySmokeEffect(TracerEndPoint);
}

