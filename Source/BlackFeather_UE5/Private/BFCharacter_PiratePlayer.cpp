// Fill out your copyright notice in the Description page of Project Settings.

#include "BFCharacter_PiratePlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#pragma region Initialization Logic
ABFCharacter_PiratePlayer::ABFCharacter_PiratePlayer(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	InitializeCamera();
}

void ABFCharacter_PiratePlayer::BeginPlay()
{
	Super::BeginPlay();
	ResetCameraFocus();
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
	PlayerInputComponent->BindAxis(FName("CameraRotatePitch"), this, &ABFCharacter_PiratePlayer::RotateCamPitch);
	PlayerInputComponent->BindAxis(FName("CameraRotateYaw"), this, &ABFCharacter_PiratePlayer::RotateCamYaw);
	PlayerInputComponent->BindAction(FName("ResetCamera"), IE_Pressed, this, &ABFCharacter_PiratePlayer::ResetCameraFocus);
}
#pragma endregion

#pragma region Camera Logic
void ABFCharacter_PiratePlayer::ResetCameraFocus()
{
	if (!CameraArm) { return; }

	CameraArm->SetRelativeRotation(FRotator::ZeroRotator);
	LookAtCameraFocusPoint();
}

void ABFCharacter_PiratePlayer::LookAtCameraFocusPoint()
{
	const USkeletalMeshComponent* pirateMesh = GetMesh();
	if (!pirateMesh || !CameraArm) { return; }

	FVector vSocketLocation;
	FRotator rSocketRotation;
	pirateMesh->GetSocketWorldLocationAndRotation("Pirate_CameraFocusPoint", vSocketLocation, rSocketRotation);

	//Set Camera to point at Camera Focus point
	const FVector vCameraToSocket = vSocketLocation - CameraComponent->GetComponentLocation();
	const FRotator rCameraToSocketRelativeRotation = vCameraToSocket.Rotation() - GetActorRotation();
	const FRotator rNewCameraFocusRotation = FRotator(rCameraToSocketRelativeRotation.Pitch, rCameraToSocketRelativeRotation.Yaw, 0.0f);
	CameraComponent->SetWorldRotation(rNewCameraFocusRotation);
}

void ABFCharacter_PiratePlayer::RotateCamYaw(const float fInputValue)
{
	const USkeletalMeshComponent* pirateMesh = GetMesh();
	if (!pirateMesh || !CameraArm || fInputValue == 0.0f) { return; }

	const float fYawRotateAmount = fInputValue * CameraYawRotationAmount;
	const FRotator fNewRotation = FRotator(0.0f, fYawRotateAmount, 0.0f);
	CameraArm->AddRelativeRotation(fNewRotation);

	LookAtCameraFocusPoint();
}

void ABFCharacter_PiratePlayer::RotateCamPitch(const float fInputValue)
{
	if (!CameraArm || fInputValue == 0.0f) { return; }

	const float fCamRotateAmount = InvertCamera ? -(fInputValue*CameraPitchRotationAmount) : (fInputValue*CameraPitchRotationAmount);
	const FRotator rCameraPitchRotation = FRotator(fCamRotateAmount, 0.0f, 0.0f);
	CameraArm->AddRelativeRotation(rCameraPitchRotation);

	LookAtCameraFocusPoint();
}

void ABFCharacter_PiratePlayer::InitializeCamera()
{
	USkeletalMeshComponent* pirateMesh = GetMesh();
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(FName("CameraArm"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("PirateCamera"));
	if (!CameraArm && !CameraComponent && !pirateMesh) { return; }

	CameraArm->SetupAttachment(RootComponent);
	CameraComponent->AttachToComponent(CameraArm, FAttachmentTransformRules::KeepRelativeTransform);
	
	CameraArm->TargetArmLength = 300.0f;
	CameraArm->SocketOffset = FVector(0,0, 200.0f);

	FVector vSocketLocation;
	FRotator rSocketRotation;
	pirateMesh->GetSocketWorldLocationAndRotation("Pirate_CameraFocusPoint", vSocketLocation, rSocketRotation);

	const FVector vCameraToSocket = vSocketLocation - CameraComponent->GetComponentLocation();
	const FRotator rCameraToSocketRelativeRotation = vCameraToSocket.Rotation() - GetActorRotation();
	const FRotator rNewCameraFocusRotation = FRotator(CameraComponent->GetRelativeRotation().Pitch, rCameraToSocketRelativeRotation.Yaw, 0.0f);
	CameraComponent->SetRelativeRotation(rNewCameraFocusRotation);
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
