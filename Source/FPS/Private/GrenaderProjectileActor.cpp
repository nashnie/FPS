// Fill out your copyright notice in the Description page of Project Settings.

#include "GrenaderProjectileActor.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGrenaderProjectileActor::AGrenaderProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComp->InitSphereRadius(ExplodeRadius);

	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->BodyInstance.SetCollisionProfileName("Projectile");
	SphereComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	SphereComp->CanCharacterStepUpOn = ECB_No;
	SphereComp->SetGenerateOverlapEvents(true);

	SphereComp->OnComponentHit.AddDynamic(this, &AGrenaderProjectileActor::OnHit);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AGrenaderProjectileActor::MyOverlapFunction);
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComp->SetupAttachment(SphereComp);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovementComp->UpdatedComponent = SphereComp;
	ProjectileMovementComp->InitialSpeed = 3000.f;
	ProjectileMovementComp->MaxSpeed = 3000.f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	SphereCollisionEnabled = false;
	DelayExplodeTime = 1.0f;
	ExplodeRadius = 50.0f;

	UE_LOG(LogTemp, Log, TEXT("AGrenaderProjectileActor Initialize..."));
}

// Called when the game starts or when spawned
void AGrenaderProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGrenaderProjectileActor::Explode, DelayExplodeTime, false);

	UE_LOG(LogTemp, Log, TEXT("AGrenaderProjectileActor BeginPlay..."));
}

void AGrenaderProjectileActor::Explode()
{
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollisionEnabled = true;

	FRadialDamageParams Params;
	Params.BaseDamage = 100.0f;
	Params.OuterRadius = ExplodeRadius;

	FRadialDamageEvent DamageEvent;
	DamageEvent.Params = Params;
	DamageEvent.Origin = GetActorLocation();

	TakeDamage(100.0f, DamageEvent, GetInstigatorController(), this);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeEffect, GetActorLocation(), GetActorRotation(), true);

	UE_LOG(LogTemp, Log, TEXT("AGrenaderProjectileActor Explode..."));

	Destroy();
}

void AGrenaderProjectileActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovementComp->UpdatedComponent = nullptr;
	ProjectileMovementComp->InitialSpeed = 0.0f;

	UE_LOG(LogTemp, Log, TEXT("OnHit %s"), *OtherComp->GetName());
}

void AGrenaderProjectileActor::MyOverlapFunction(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("MyOverlapFunction %s"), *OtherComp->GetName());
}

// Called every frame
void AGrenaderProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*if (SphereCollisionEnabled)
	{
		TArray<AFPSCharacter*> CollectedActors;
		MeshComp->GetOverlappingComponents(CollectedActors);
		for (int32 i = 0; i < CollectedActors.Num(); ++i)
		{
			AFPSCharacter* anActor = CollectedActors[i];
			if (anActor)
			{
				FRadialDamageParams Params;
				Params.BaseDamage = 100.0f;
				Params.OuterRadius = ExplodeRadius;

				FRadialDamageEvent DamageEvent;
				DamageEvent.Params = Params;
				DamageEvent.Origin = GetActorLocation();

				anActor->TakeDamage(100.0f, DamageEvent);
			}
		}
	}*/
}

