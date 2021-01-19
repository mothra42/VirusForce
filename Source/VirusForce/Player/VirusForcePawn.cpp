// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "VirusForcePawn.h"
#include "VirusForceProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "../NPC/KillerTCell.h"
#include "../GameMode/VirusForceGameMode.h"

const FName AVirusForcePawn::MoveForwardBinding("MoveForward");
const FName AVirusForcePawn::MoveRightBinding("MoveRight");
const FName AVirusForcePawn::FireForwardBinding("FireForward");
const FName AVirusForcePawn::FireRightBinding("FireRight");
const FName AVirusForcePawn::SpawnKillerTCell("SpawnKillerTCell");

AVirusForcePawn::AVirusForcePawn()
{	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CellWall(TEXT("/Game/Geometry/Meshes/PlayerMeshes/Player/CellWall"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Core(TEXT("/Game/Geometry/Meshes/PlayerMeshes/Player/Core"));
	// Create the mesh component
	CellWallComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CellWall"));
	CoreComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Core"));
	CoreComponent->SetupAttachment(CellWallComponent);
	//CoreComponent->AttachToComponent(CellWallComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("CoreSocket"));
	RootComponent = CellWallComponent;
	CellWallComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	CellWallComponent->SetStaticMesh(CellWall.Object);
	
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

	// Movement
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(130.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;
}

void AVirusForcePawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);
	PlayerInputComponent->BindAction("SpawnKillerTCell", IE_Pressed, this, &AVirusForcePawn::SpawnKillerTCellInWorld);
	PlayerInputComponent->BindAction("SwitchAntibodyTypeUp", IE_Pressed, this, &AVirusForcePawn::SwitchAntibodyTypeUp);
	PlayerInputComponent->BindAction("SwitchAntibodyTypeDown", IE_Pressed, this, &AVirusForcePawn::SwitchAntibodyTypeDown);
}

void AVirusForcePawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bCanMove)
	{
		// Find movement direction
		const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
		const float RightValue = GetInputAxisValue(MoveRightBinding);

		// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
		const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);
		// Calculate  movement
		const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

		PlayerVelocity = MoveDirection * MoveSpeed;

		// If non-zero size, move this actor
		if (Movement.SizeSquared() > 0.0f)
		{
			const FRotator NewRotation = Movement.Rotation();
			FHitResult Hit(1.f);
			RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

			//This is here so that the ship can move even if it is hitting a wall or other object
			if (Hit.IsValidBlockingHit())
			{
				const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
				const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
				RootComponent->MoveComponent(Deflection, NewRotation, true);
			}
		}

		// Create fire direction vector
		const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
		const float FireRightValue = GetInputAxisValue(FireRightBinding);
		const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.f);

		// Try and fire a shot
		FireShot(FireDirection);
	}
}

void AVirusForcePawn::BeginPlay()
{
	Super::BeginPlay();

	AvailableSocketNames = CellWallComponent->GetAllSocketNames();
}

void AVirusForcePawn::SwitchAntibodyTypeUp()
{
	if (ProjectileIndexTracker >= 3)
	{
		ProjectileIndexTracker = 0;
	}
	else
	{
		ProjectileIndexTracker++;
	}
	ProjectileClass = AntibodyTypes[ProjectileIndexTracker];
	
	SwitchProjectileTypeDelegate.Broadcast(ProjectileIndexTracker);
}

void AVirusForcePawn::SwitchAntibodyTypeDown()
{
	if (ProjectileIndexTracker <= 0)
	{
		ProjectileIndexTracker = 3;
	}
	else
	{
		ProjectileIndexTracker--;
	}
	ProjectileClass = AntibodyTypes[ProjectileIndexTracker];
	SwitchProjectileTypeDelegate.Broadcast(ProjectileIndexTracker);
}

void AVirusForcePawn::SetProjectile(TSubclassOf<AVirusForceProjectile> ProjectileToSet)
{
	ProjectileClass = ProjectileToSet;
}

void AVirusForcePawn::FireShot(FVector FireDirection)
{
	// If it's ok to fire again
	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);
			UWorld* const World = GetWorld();
			if (World != NULL && ProjectileClass != nullptr)
			{
				// spawn the projectile
				//TSubclassOf<AVirusForceProjectile>* Projectile = World->SpawnActor<TSubclassOf<AVirusForceProjectile>>(ProjectileClass, SpawnLocation, FireRotation);
				World->SpawnActor<AVirusForceProjectile>(ProjectileClass, SpawnLocation, FireRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AVirusForcePawn::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				//UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
		}
	}
}

void AVirusForcePawn::ShotTimerExpired()
{
	bCanFire = true;
}

//TODO when input is pressed spawn killer T Cell, where is to be decided whether its a fixed position or if it should spawn in a random position.
void AVirusForcePawn::SpawnKillerTCellInWorld()
{
	//TODO find random location on plane
	FVector SpawnLocation = FVector(70.f, 1780.f, 0.f);
	FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
	UWorld* const World = GetWorld();
	if (World != NULL && KillerTCellClass != nullptr)
	{
		// spawn the killer t cell
		World->SpawnActor<AKillerTCell>(KillerTCellClass, SpawnLocation, SpawnRotation);
	}
}

//Lives are stored in the game mode, calls game mode to run lost life function
void AVirusForcePawn::LoseLife()
{

	UWorld* World = GetWorld();
	if (World != NULL)
	{
		AVirusForceGameMode* GameMode = Cast<AVirusForceGameMode>(World->GetAuthGameMode());
		if (GameMode != nullptr && !bGodModeOn)
		{
			GameMode->ResetGameOnLifeLost(World);
		}
	}
}

//used to set all mesh components of mesh invisible
void AVirusForcePawn::SetPlayerInvisible()
{
	CellWallComponent->SetVisibility(false);
	CoreComponent->SetVisibility(false);
	SetVisibilityDelegate.Broadcast();
}