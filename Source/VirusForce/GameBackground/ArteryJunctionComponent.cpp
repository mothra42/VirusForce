// Copyright Adam Farmer 2020

#include "ArteryJunctionComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UArteryJunctionComponent::UArteryJunctionComponent()
{
	//setup collision mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CollisionBox(TEXT("/Game/Geometry/Meshes/1M_Cube"));
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionMesh"));
	CollisionMesh->SetStaticMesh(CollisionBox.Object);
	CollisionMesh->SetWorldScale3D(FVector(0.5, 0.5, 0.5));
	CollisionMesh->bHiddenInGame = true;
	CollisionMesh->SetupAttachment(this);
	CollisionMesh->SetCollisionProfileName(FName("OverlapAllDynamic"));
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Called when the game starts
void UArteryJunctionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	TArray<UPrimitiveComponent*> OverlappingComponents;
	CollisionMesh->GetOverlappingComponents(OverlappingComponents);
	UE_LOG(LogTemp, Warning, TEXT("%s component has %i overlapping components"), *this->GetName(), OverlappingComponents.Num());
	
}


// Called every frame
void UArteryJunctionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TArray<UPrimitiveComponent*> OverlappingComponents;
	CollisionMesh->GetOverlappingComponents(OverlappingComponents);
}

void UArteryJunctionComponent::SetupConnection(USplineComponent* SplineComponent, int32 SplinePoint, FVector NewLocation)
{
	SplineComponent->SetLocationAtSplinePoint(SplinePoint, NewLocation, ESplineCoordinateSpace::World, true);
	SplineComponent->bSplineHasBeenEdited = true;
}

UStaticMeshComponent* UArteryJunctionComponent::GetCollisionMesh()
{
	return CollisionMesh;
}

void UArteryJunctionComponent::SetupArteryArrayAndMap(USplineComponent* SplineComponent, int32 SplinePoint)
{
	ArteryArray.Add(SplineComponent);
	ArteryLocationMap.Add(SplineComponent, SplinePoint);
}