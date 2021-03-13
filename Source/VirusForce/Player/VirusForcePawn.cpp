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
#include "../NPC/Virus.h"
#include "../GameMode/VirusForceGameMode.h"
#include "../NPC/MarkedVirusComponent.h"

const FName AVirusForcePawn::MoveForwardBinding("MoveForward");
const FName AVirusForcePawn::MoveRightBinding("MoveRight");
const FName AVirusForcePawn::FireForwardBinding("FireForward");
const FName AVirusForcePawn::FireRightBinding("FireRight");
const FName AVirusForcePawn::SpawnKillerTCell("SpawnKillerTCell");

AVirusForcePawn::AVirusForcePawn()
{	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CellWall(TEXT("/Game/Geometry/Meshes/PlayerMeshes/Player/CellWall"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Core(TEXT("/Game/Geometry/Meshes/PlayerMeshes/Player/Core"));
	CoreComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Core"));
	RootComponent = CoreComponent;
	CoreComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	CoreComponent->SetStaticMesh(Core.Object);
	CoreComponent->SetWorldScale3D(FVector(1.5, 1.5, 1.5));
	
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
	PlayerInputComponent->BindAction("SpawnKillerTCell", IE_Pressed, this, &AVirusForcePawn::SetupKillerTCellSpawn);
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
			MovementComponent->AddInputVector(Movement, true);

			//This is here so that the ship can move even if it is hitting a wall or other object
			if (Hit.IsValidBlockingHit())
			{
				const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
				const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
				MovementComponent->AddInputVector(Deflection, true);
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
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
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
				World->SpawnActor<AVirusForceProjectile>(ProjectileClass, SpawnLocation, FireRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AVirusForcePawn::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				//TODO add sound effect
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

void AVirusForcePawn::SetupKillerTCellSpawn()
{
	TArray<AActor*> KillerCellArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKillerTCell::StaticClass(), KillerCellArray);
	//only spawn new killer t cells if none are in the arena
	if (KillerCellArray.Num() <= 0)
	{
		//find number of viruses in arena to determine how many killer cells to spawn
		TArray<AActor*> VirusArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVirus::StaticClass(), VirusArray);
		int32 NumVirusesInArena = VirusArray.Num();
		int32 NumKillerCellsToSpawn = DetermineNumKillerCellsToSpawn(NumVirusesInArena);
		SpawnKillerTCellInWorld(NumKillerCellsToSpawn);
	}
}

int32 AVirusForcePawn::DetermineNumKillerCellsToSpawn(int32 NumVirusesInArena)
{
	if (NumVirusesInArena <= 15)
	{
		return 1;
	}
	else if (NumVirusesInArena <= 40)
	{
		return 2;
	}
	//default to spawning 3 killer cells
	return 3;
}

void AVirusForcePawn::SpawnKillerTCellInWorld(int32 SpawnNumber)
{
	FVector SpawnLocation;
	FRotator SpawnRotation = FRotator();
	TArray<AKillerTCell*> KillerTCellArray;
	UWorld* const World = GetWorld();
	if (World != NULL && KillerTCellClass != nullptr)
	{
		switch (SpawnNumber)
		{
		case 1:
			SpawnLocation = FVector(0.f, 0.f, -11400.f);
			KillerTCellArray.Add(World->SpawnActor<AKillerTCell>(KillerTCellClass, SpawnLocation, SpawnRotation));
			break;
		case 2:
			SpawnLocation = FVector(-250.f, 0.f, -11400.f);
			KillerTCellArray.Add(World->SpawnActor<AKillerTCell>(KillerTCellClass, SpawnLocation, SpawnRotation));
			SpawnLocation = FVector(250.f, 0.f, -11400.f);
			KillerTCellArray.Add(World->SpawnActor<AKillerTCell>(KillerTCellClass, SpawnLocation, SpawnRotation));
			break;
		case 3:
			SpawnLocation = FVector(-250.f, 250.f, -11400.f);
			KillerTCellArray.Add(World->SpawnActor<AKillerTCell>(KillerTCellClass, SpawnLocation, SpawnRotation));
			SpawnLocation = FVector(250.f, 250.f, -11400.f);
			KillerTCellArray.Add(World->SpawnActor<AKillerTCell>(KillerTCellClass, SpawnLocation, SpawnRotation));
			SpawnLocation = FVector(0.f, -250.f, -11400.f);
			KillerTCellArray.Add(World->SpawnActor<AKillerTCell>(KillerTCellClass, SpawnLocation, SpawnRotation));
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Unexpected number of viruses spawning"));
			return;
		}
		AVirusForceGameMode* GameMode = Cast<AVirusForceGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode != nullptr)
		{
			UMarkedVirusComponent* MarkedVirusComponent = GameMode->GetMarkedVirusComponent();
			MarkedVirusComponent->DistributeMarkedViruses(SpawnNumber, KillerTCellArray);
		}
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
	CoreComponent->SetVisibility(false);
	SetVisibilityDelegate.Broadcast();
}