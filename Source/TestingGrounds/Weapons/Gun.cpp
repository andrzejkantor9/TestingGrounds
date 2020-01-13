// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Animation/AnimInstance.h"
#include "BallProjectile.h"
#include "Kismet/GameplayStatics.h"

#include "TimerManager.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	//FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::AllowShooting()
{
	bCanShoot = true;
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ShootTimerHandle);
	}
}


void AGun::OnFire()
{
	// try and fire a projectile
	if (!bCanShoot)
	{
		return;
	}
	else
	{
		bCanShoot = false;
		FTimerDelegate ShootingTimerDelegate;
		ShootingTimerDelegate.BindUFunction(this, FName("AllowShooting"));

		GetWorldTimerManager().SetTimer(ShootTimerHandle, ShootingTimerDelegate, .1f, false, .4f);

	}
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			/*
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<ABallProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			*/
			
			FRotator SpawnRotation;
			FVector SpawnLocation;
			if (ensure(FP_MuzzleLocation))
			{
				SpawnRotation = FP_MuzzleLocation->GetComponentRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				SpawnLocation = FP_MuzzleLocation->GetComponentLocation();
			}				

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<ABallProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FPFireAnimation != nullptr && FPAnimInstance != nullptr)
	{
		FPAnimInstance->Montage_Play(FPFireAnimation, 1.f);
	}
	if (TPFireAnimation != nullptr && TPAnimInstance != nullptr)
	{
		TPAnimInstance->Montage_Play(TPFireAnimation, 1.f);
	}
}
