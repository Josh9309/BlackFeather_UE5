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
		const FAttachmentTransformRules cameraAttachRules(EAttachmentRule::KeepRelative, false);
		CameraComponent->AttachToComponent(PirateMesh, cameraAttachRules);
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
	//PlayerInputComponent->BindAxis(FName("CameraRotatePitch"), this, &ABFCharacter_PiratePlayer::RotateCamPitch);
	//PlayerInputComponent->BindAxis(FName("CameraRotateYaw"), this, &ABFCharacter_PiratePlayer::RotateCamYaw);
}
#pragma endregion

#pragma region Camera Logic
void ABFCharacter_PiratePlayer::RotateCamYaw(const float fInputValue)
{
	const USkeletalMeshComponent* pirateMesh = GetMesh();
	if (!pirateMesh || !CameraComponent || fInputValue == 0.0f) { return; }

	FVector vSocketLocation;
	FRotator rSocketRotation;
	pirateMesh->GetSocketWorldLocationAndRotation("Pirate_CameraFocusPoint", vSocketLocation, rSocketRotation);
	DrawDebugSphere(GetWorld(), vSocketLocation, 100, 20, FColor::Blue);

	//Rotate Camera Location
	const FRotator rCamYawRotation = FRotator(0.0f, fInputValue * CameraYawRotationAmount, 0.0f);
	const FVector vSocketToCurrentCamLocation = CameraComponent->GetComponentLocation() - vSocketLocation;
	const FVector vToNewCamLocation = rCamYawRotation.RotateVector(vSocketToCurrentCamLocation);
	CameraComponent->SetRelativeLocation(vToNewCamLocation);


	//Set Camera to point at Camera Focus point
	const FVector vCameraToSocket = vSocketLocation - CameraComponent->GetComponentLocation();
	const FRotator rCameraToSocketRelativeRotation = vCameraToSocket.Rotation() - GetActorRotation();
	const FRotator rNewCameraFocusRotation = FRotator(CameraComponent->GetRelativeRotation().Pitch, rCameraToSocketRelativeRotation.Yaw, 0.0f);
	CameraComponent->SetRelativeRotation(rNewCameraFocusRotation);
}

void ABFCharacter_PiratePlayer::RotateCamPitch(const float fInputValue)
{
	if (!CameraComponent || fInputValue == 0.0f) { return; }

	const float fCamRotateAmount = InvertCamera ? -(fInputValue*CameraPitchRotationAmount) : (fInputValue*CameraPitchRotationAmount);
	const FRotator rCameraPitchRotation = FRotator(fCamRotateAmount, 0.0f, 0.0f);
	CameraComponent->AddRelativeRotation(rCameraPitchRotation);
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
