
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BFCharacter_PiratePlayer.generated.h"

UCLASS()
class BLACKFEATHER_UE5_API ABFCharacter_PiratePlayer : public ACharacter
{
	GENERATED_BODY()

#pragma region Initialization Logic
public:
	// Sets default values for this character's properties
	ABFCharacter_PiratePlayer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
#pragma endregion

#pragma region Input System Logic
protected:
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma endregion

#pragma region Camera Logic
public:
	UFUNCTION(BlueprintCallable)
	void ResetCameraFocus();

	UFUNCTION(BlueprintCallable)
	void LookAtCameraFocusPoint();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool InvertCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CameraYawRotationAmount = 1.0f; //Stored in Degrees

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CameraPitchRotationAmount = 0.5f; //Stored in Degrees

	const bool UseCameraForward = true;

	void RotateCamYaw(const float fInputValue);
	void RotateCamPitch(const float fInputValue);

private:
	void InitializeCamera();

#pragma endregion

#pragma region Movement Logic
public:
	UFUNCTION(BlueprintPure)
	FVector GetCharacterForward() const;

	UFUNCTION(BlueprintPure)
	FVector GetCharacterRight() const;

protected:
	void MoveForward(const float fInputValue);
	void MoveRight(const float fInputValue);
	void MoveJump(const float fInputValue);
#pragma endregion

};
