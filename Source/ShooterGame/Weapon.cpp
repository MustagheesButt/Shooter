// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

AWeapon::AWeapon() :
	ThrowWeaponTime(0.7f),
	bFalling(false)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Keep the weapon upright
	if (GetItemState() == EItemState::EIS_Falling && bFalling)
	{
		FRotator MeshRotation{ 0.f, GetMesh()->GetComponentRotation().Yaw, 0.f };
		GetMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
}

void AWeapon::ThrowWeapon()
{
	FRotator MeshRotation{ 0.f, GetMesh()->GetComponentRotation().Yaw, 0.f };
	GetMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

	const FVector MeshForward{ GetMesh()->GetForwardVector() };
	const FVector MeshRight{ GetMesh()->GetRightVector() };
	// Direction in which we throw the weapon
	FVector ImpulseDir = MeshRight.RotateAngleAxis(-20.f, MeshForward);

	float RandomRotation = FMath::FRandRange(20.f, 40.f);
	ImpulseDir = ImpulseDir.RotateAngleAxis(RandomRotation, FVector(0.f, 0.f, 1.f));
	ImpulseDir *= 5000.f;

	GetMesh()->AddImpulse(ImpulseDir);

	bFalling = true;
	GetWorldTimerManager().SetTimer(ThrowWeaponTimer, this, &AWeapon::StopFalling, ThrowWeaponTime);
}

void AWeapon::DecrementAmmo()
{
	if (Ammo > 0)
	{
		Ammo--;
	}
}

void AWeapon::StopFalling()
{
	bFalling = false;
	SetItemState(EItemState::EIS_Pickup);
}
