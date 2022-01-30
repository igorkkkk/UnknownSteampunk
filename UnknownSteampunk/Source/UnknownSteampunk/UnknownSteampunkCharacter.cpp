// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnknownSteampunkCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AUnknownSteampunkCharacter::AUnknownSteampunkCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	SetActorEnableCollision(true);
	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 700.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
	JumpMaxCount = 2;


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUnknownSteampunkCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUnknownSteampunkCharacter::MoveRight);
	PlayerInputComponent->BindAction("Soaring",IE_Pressed,this,&AUnknownSteampunkCharacter::Soaring);
	PlayerInputComponent->BindAction("Soaring",IE_Released,this,&AUnknownSteampunkCharacter::StopSoaring);

}

void AUnknownSteampunkCharacter::Soaring()
{
	QKey = true;
}
void AUnknownSteampunkCharacter::StopSoaring()
{
	QKey = false;
}
void AUnknownSteampunkCharacter::MoveRight(float Value)
{

	// Apply the input to the character motion
	float MoveValue = Value;
	if(TurnJump%2)
	{
		MoveValue= -MoveValue;
	}
    if(Value)
    {
	    AxisMoving = true;
    }
	else
	{
		AxisMoving = false;
	}
	AddMovementInput(FVector(0.0f, -1.0f, 0.0f), MoveValue);
}


void AUnknownSteampunkCharacter::Tick(float DeltaSeconds)
{

	
	Super::Tick(DeltaSeconds);
	////AddActorLocalOffset(FVector(0.f,0.f,1.0f),true);
	UpdateCharacter();	
}

void AUnknownSteampunkCharacter::UpdateCharacter()
{
	const FVector PlayerVelocity = GetVelocity();	
	float FallDirection = PlayerVelocity.Z;
	//if Q key was pressed nd we falling we can soaring
	if(QKey&&(FallDirection<0))
	{
		GetCharacterMovement()->GravityScale = Gravity;
	}
	else
	{
		GetCharacterMovement()->GravityScale = 2;
	}
	
	// Now setup the rotation of the controller based on the direction we are travelling
	float TravelDirection = PlayerVelocity.Y;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
	
	// changing second jump vector
	if((JumpCurrentCount>1)&&(TravelDirection==0)&&AxisMoving)//TODO
	{
		if(IsRootComponentCollisionRegistered())
		{
			TurnJump++;  
			JumpCurrentCount--;
		}
	}
	//if we hit the floor
	else if((JumpCurrentCount==0))
	{
		TurnJump = 0;
	}
}
