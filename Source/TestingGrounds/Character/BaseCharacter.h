// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class TESTINGGROUNDS_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	/** First person camera */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

protected:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
		class USkeletalMeshComponent* FirstPersonArmsComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		TSubclassOf<class AGun> GunBlueprint;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		AGun* Gun = nullptr;
public:
	UFUNCTION(BlueprintCallable, Category = "Shooting")
		void Fire();
public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};