// Fill out your copyright notice in the Description page of Project Settings.

#include "Parcial1/PhotoObjectActor.h"
#include "Kismet/GameplayStatics.h"
#include "Parcial1/PhotoActorComponent.h" // Necesitamos el header del componente
#include "GameFramework/Character.h"

// Sets default values
APhotoObjectActor::APhotoObjectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // 1. Creamos la raíz de la escena explícita
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;

    // 2. Creamos el Mesh y lo enganchamos a la raíz
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent); // Se vuelve hijo de la raíz

    // 3. Configuramos la colisión en el canal que usa tu Raycast (usualmente Visibility)
    MeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
}

// Called when the game starts or when spawned
void APhotoObjectActor::BeginPlay()
{
	Super::BeginPlay();

    TObjectPtr<AActor> PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (PlayerCharacter)
    {
        // Buscamos el PhotoActorComponent dentro del personaje
        TObjectPtr<UPhotoActorComponent> PhotoComp = PlayerCharacter->FindComponentByClass<UPhotoActorComponent>();

        if (PhotoComp)
        {
            PhotoComp.Get()->OnPhotoRegistered.AddDynamic(this, &APhotoObjectActor::AlSerFotografiado);
        }
    }
}

// Called every frame
void APhotoObjectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UDA_PhotoObject* APhotoObjectActor::GetPhotoData_Implementation() const
{
    return this->DatosFotografia;
}

void APhotoObjectActor::AlSerFotografiado(UDA_PhotoObject* ObjetoRegistrado)
{
    if (ObjetoRegistrado && ObjetoRegistrado == DatosFotografia)
    {
        bHasBeenPhotographed = true;
        UE_LOG(LogTemp, Log, TEXT("El PhotoObjectActor '%s' ha cambiado a FOTOGRAFIADO."), *GetName());
    }
}