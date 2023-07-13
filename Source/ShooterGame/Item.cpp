// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "ShooterCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AItem::AItem():
	ItemName(FString("Default")),
	ItemCount(0),
	ItemRarity(EItemRarity::EIR_Common),
	ItemState(EItemState::EIS_Pickup)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(Mesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(Mesh);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(GetRootComponent());

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	PickupWidget->SetVisibility(false);
	
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

	SetActiveStars();

	SetItemState(ItemState);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if (ShooterCharacter)
		{
			ShooterCharacter->UpdateOverlappedItemCount(1);
		}
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if (ShooterCharacter)
		{
			ShooterCharacter->UpdateOverlappedItemCount(-1);
		}
	}
}

void AItem::SetActiveStars()
{
	for (int i = 1; i <= 5; i++)
	{
		if (i <= int(ItemRarity) + 1)
		{
			ActiveStars.Add(true);
		}
		else
		{
			ActiveStars.Add(false);
		}
	}
}

void AItem::SetItemProperties(EItemState State)
{
	switch (State)
	{
	case EItemState::EIS_Pickup:
		Mesh->SetSimulatePhysics(false);
		Mesh->SetEnableGravity(false);
		Mesh->SetVisibility(true);
		Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;

	case EItemState::EIS_Equipped:
		PickupWidget->SetVisibility(false);
		Mesh->SetSimulatePhysics(false);
		Mesh->SetEnableGravity(false);
		Mesh->SetVisibility(true);
		Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EItemState::EIS_Falling:
		Mesh->SetSimulatePhysics(true);
		Mesh->SetEnableGravity(true);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AItem::SetItemState(EItemState State)
{
	this->ItemState = State;
	SetItemProperties(State);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

