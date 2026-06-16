// Copyright Epic Games, Inc. All Rights Reserved.

#include "SistemaDeFotografiaCharacter.h"
#include "Parcial1/PhotoInterface.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SistemaDeFotografia.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"

ASistemaDeFotografiaCharacter::ASistemaDeFotografiaCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	// Actor component "Photo Component";
	PhotoComponent = CreateDefaultSubobject<UPhotoActorComponent>(TEXT("PhotoComponentInstance"));
}

void ASistemaDeFotografiaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASistemaDeFotografiaCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASistemaDeFotografiaCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASistemaDeFotografiaCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASistemaDeFotografiaCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ASistemaDeFotografiaCharacter::LookInput);

		// Take Photo
		EnhancedInputComponent->BindAction(TakePhotoAction, ETriggerEvent::Started, this, &ASistemaDeFotografiaCharacter::TakePhoto);

		// Show Index
		EnhancedInputComponent->BindAction(ShowCodexAction, ETriggerEvent::Started, this, &ASistemaDeFotografiaCharacter::ShowCodex);
	}
	else
	{
		UE_LOG(LogSistemaDeFotografia, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASistemaDeFotografiaCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void ASistemaDeFotografiaCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void ASistemaDeFotografiaCharacter::TakePhoto(const FInputActionValue& Value)
{
	// Origen y dirección del rayo;
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();

	// Alcance del rayo (15 metros);
	float RayLength = 1500.0f;
	FVector End = Start + (ForwardVector * RayLength);

	// Configuración del Raycast;
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Esto evita que el rayo choque con el personaje.

	// Raycast
	bool bHasHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility, // Canal de colisión por defecto (??? Revisar)
		QueryParams
	);

	// Se pintará de verde si hace colision y de rojo si no, duración de 3 segundos;
	DrawDebugLine(GetWorld(), Start, End, bHasHit ? FColor::Green : FColor::Red, false, 3.0f, 0, 1.5f);

	if (bHasHit && HitResult.GetActor())
	{
		AActor* ActorHit = HitResult.GetActor();

		// Pregunta si implementa la interfaz
		if (ActorHit->GetClass()->ImplementsInterface(UPhotoInterface::StaticClass()))
		{
			// Paso el 'ActorHit' como parámetro para que Unreal busque la implementación;
			UDA_PhotoObject* PhotoObjectData = IPhotoInterface::Execute_GetPhotoData(ActorHit);

			if (PhotoObjectData)
			{
				if (PhotoComponent) // Validación de que el character tenga asignado el ActorComponent "PhotoActorComponent"
				{
					PhotoComponent->SavePhotoInCodex(PhotoObjectData);
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Data asset vacío, pero la interfaz funciona"));
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No voy a gastar la memoria de mi camara en esto, no fotografiaste nada"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No voy a gastar la memoria de mi camara en esto, no fotografiaste nada"));
	}
}

void ASistemaDeFotografiaCharacter::ShowCodex(const FInputActionValue& Value) 
{
	if (PhotoComponent->PhotoCodex.Num() == 0) {
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Green,
			TEXT("El Codice esta vacio"));
	}
	else {
		for (int32 i = 0; i < PhotoComponent->PhotoCodex.Num(); i++)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.f,
				FColor::Green,
				FString::Printf(TEXT("Entrada del codice: %s"), *PhotoComponent->PhotoCodex[i]->NombreObjeto));
		}

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Codice: "));
	}
	
}

void ASistemaDeFotografiaCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ASistemaDeFotografiaCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void ASistemaDeFotografiaCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void ASistemaDeFotografiaCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}
