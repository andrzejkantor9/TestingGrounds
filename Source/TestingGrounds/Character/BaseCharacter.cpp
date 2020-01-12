// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "../Weapons/Gun.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCameraComponent"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // FVector(-26.057276f, -0.415966f, 54.459229f)
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	FirstPersonArmsComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonArmsComponent"));
	FirstPersonArmsComponent->SetOnlyOwnerSee(true);
	FirstPersonArmsComponent->SetupAttachment(FirstPersonCameraComponent);
	FirstPersonArmsComponent->bCastDynamicShadow = false;
	FirstPersonArmsComponent->CastShadow = false;
	FirstPersonArmsComponent->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	FirstPersonArmsComponent->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	if (ensure(GunBlueprint))
	{
		Gun = GetWorld()->SpawnActor<AGun>(GunBlueprint);
		Gun->AttachToComponent(FirstPersonArmsComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint")); //skeleton is not yet created in constructor
		Gun->AnimInstance = FirstPersonArmsComponent->GetAnimInstance();
		// Bind fire event
		//InputComponent->BindAction("Fire", IE_Pressed, Gun, &AGun::OnFire);
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::Fire()
{
	Gun->OnFire();
}