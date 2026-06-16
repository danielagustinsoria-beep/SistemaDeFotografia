// Fill out your copyright notice in the Description page of Project Settings.


#include "Parcial1/PhotoActorComponent.h"

// Sets default values for this component's properties
UPhotoActorComponent::UPhotoActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPhotoActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPhotoActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPhotoActorComponent::SavePhotoInCodex(TObjectPtr<UDA_PhotoObject> NewPhotoObject)
{
    if (NewPhotoObject)
    {
        // No guardar si ya está en la lista
        if (!PhotoCodex.Contains(NewPhotoObject))
        {
            PhotoCodex.Add(NewPhotoObject);
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("¡Codice actualizado! Has registrado: %s"), *NewPhotoObject->NombreObjeto));
            
            // Disparar evento-delegate para actualizar la variable de estado del objeto.
            OnPhotoRegistered.Broadcast(NewPhotoObject);
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Ya fotografiaste este objeto"));
        }
    }
}