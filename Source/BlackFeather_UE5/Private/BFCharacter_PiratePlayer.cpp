// Fill out your copyright notice in the Description page of Project Settings.

#include "BFCharacter_PiratePlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#pragma region Initialization Logic
ABFCharacter_PiratePlayer::ABFCharacter_PiratePlayer(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	USkeletalMeshComponent* PirateMesh = GetMesh();
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("PirateCamera"));
	if (CameraComponent && PirateMesh)
	{
		CameraComponent->SetupAttachment(PirateMesh, "Pirate_CameraFocusPoint");
	}
}

void ABFCharacter_PiratePlayer::BeginPlay()
{
	Super::BeginPlay();
}
#pragma endregion

#pragma region Input System Logic
void ABFCharacter_PiratePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("Forward"), this, &ABFCharacter_PiratePlayer::MoveForward);
	PlayerInputComponent->BindAxis(FName("Right"), this, &ABFCharacter_PiratePlayer::MoveRight);
	PlayerInputComponent->BindAxis(FName("Jump"), this, &ABFCharacter_PiratePlayer::MoveJump);
}
#pragma endregion

#pragma region Camera Logic
FVector ABFCharacter_PiratePlayer::RotateAroundPoint(const float fRotationAmount, const FVector vRotationPoint, const FVector vPointToRotate) const
{
	return FVector();
}

void ABFCharacter_PiratePlayer::RotateCamYaw(const float fInputValue)
{

}

void ABFCharacter_PiratePlayer::RotateCamPitch(const float fInputValue)
{

}
#pragma endregion

#pragma region Movement Logic
FVector ABFCharacter_PiratePlayer::GetCharacterForward() const
{
	return GetActorRotation().Vector().GetSafeNormal();
}

FVector ABFCharacter_PiratePlayer::GetCharacterRight() const
{
	return FVector::CrossProduct(FVector::UpVector, GetCharacterForward());
}

void ABFCharacter_PiratePlayer::MoveForward(const float fInputValue)
{
	const FVector vForward = (UseCameraForward && CameraComponent) ? CameraComponent->GetForwardVector() : GetCharacterForward();
	AddMovementInput(vForward.GetSafeNormal2D(), fInputValue);
}

void ABFCharacter_PiratePlayer::MoveRight(const float fInputValue)
{
	const FVector vRight = (UseCameraForward && CameraComponent) ? CameraComponent->GetRightVector() : GetCharacterRight();
	AddMovementInput(GetCharacterRight().GetSafeNormal2D(), fInputValue);
}

void ABFCharacter_PiratePlayer::MoveJump(const float fInputValue)
{
	if (fInputValue > 0.0f)
	{
		Jump();
	}
	else
	{
		StopJumping();
	}
}
#pragma endregion
