// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "FPSWeaponActor.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	WeaponAttachSocketName = "WeaponSocket";

	//UE_LOG(LogTemp, Log, TEXT("AFPSCharacter %s"), SpringArmComp->bUsePawnControlRotation ? "1" : "0");

	OnTakeRadialDamage.AddDynamic(this, &AFPSCharacter::TakeRadialDamageHandler);
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FPSWeaponActor = GetWorld()->SpawnActor<AFPSWeaponActor>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (FPSWeaponActor)
	{
		FPSWeaponActor->SetOwner(this);
		FPSWeaponActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
	}

	//UE_LOG(LogTemp, Log, TEXT("BeginPlay %s"), SpringArmComp->bUsePawnControlRotation ? "1" : "0");
}

void AFPSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		//UE_LOG(LogTemp, Log, TEXT("AddMovementInput %f"), Value);
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFPSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		//UE_LOG(LogTemp, Log, TEXT("MoveRight %f"), Value);
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFPSCharacter::StartCrouch()
{
	Crouch();
	UE_LOG(LogTemp, Log, TEXT("StartCrouch"));
}

void AFPSCharacter::EndCrouch()
{
	UnCrouch();
	UE_LOG(LogTemp, Log, TEXT("EndCrouch"));
}

void AFPSCharacter::Fire()
{
	UE_LOG(LogTemp, Log, TEXT("Fire"));
	if (FPSWeaponActor)
	{
		FPSWeaponActor->Fire();
	}
}

void AFPSCharacter::LookUp(float Val)
{
	//UE_LOG(LogTemp, Log, TEXT("AddControllerPitchInput %f%s"), Val, Controller->IsLocalPlayerController() ? TEXT("True") : TEXT("False"));
	//UE_LOG(LogTemp, Log, TEXT("AddControllerPitchInput IgnoreLookInput %s"), Controller->IsLookInputIgnored() ? TEXT("True") : TEXT("False"));
	//APlayerController* PC = Cast<APlayerController>(Controller);
	//UE_LOG(LogTemp, Log, TEXT("AddControllerPitchInput IgnoreLookInput %f"), PC->InputPitchScale);//

	//UE_LOG(LogTemp, Log, TEXT("LookUp %s"), SpringArmComp->bUsePawnControlRotation ? "1" : "0");
	Super::AddControllerPitchInput(Val);
}

void AFPSCharacter::TakeRadialDamageHandler(AActor * DamagedActor, float Damage, const UDamageType * DamageType, FVector Origin, FHitResult HitInfo, AController * InstigatedBy, AActor * DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("TakeRadialDamageHandler, %f"), Damage);
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFPSCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AFPSCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);

	UE_LOG(LogTemp, Log, TEXT("SetupPlayerInputComponent"));
}

FVector AFPSCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

