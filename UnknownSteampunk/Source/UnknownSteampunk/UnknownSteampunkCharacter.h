// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnknownSteampunkCharacter.generated.h"

UCLASS(config=Game)
class AUnknownSteampunkCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	virtual void Tick(float DeltaSeconds) override;

	bool AxisMoving = 0;
	int TurnJump = 0;
	bool QKey = 0;
	UPROPERTY(EditAnywhere,Category = "Soaring")
	double Gravity{0.01};

	//particle system
	UParticleSystem* UPart;
	UParticleSystemComponent* LeftLegParticleSystem;
	UParticleSystemComponent* RightLegParticleSystem;

	
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* SoaringAudioBase;
	UPROPERTY(EditAnywhere, Category = "Audio")
	UAudioComponent* SoaringAudioComponent;
   

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

    void Soaring();
	void StopSoaring();
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
	void UpdateCharacter();
	
	void ParticleToggle();
public:
	AUnknownSteampunkCharacter();
    virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
