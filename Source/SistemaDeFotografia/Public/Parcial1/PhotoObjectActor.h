// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DA_PhotoObject.h"
#include "Parcial1/PhotoInterface.h"
#include "PhotoObjectActor.generated.h"

class UDA_PhotoObject;

UCLASS()
class SISTEMADEFOTOGRAFIA_API APhotoObjectActor : public AActor, public IPhotoInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhotoObjectActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDA_PhotoObject> DatosFotografia;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fotografia")
	bool bHasBeenPhotographed = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void AlSerFotografiado(UDA_PhotoObject* ObjetoRegistrado);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual UDA_PhotoObject* GetPhotoData_Implementation() const override;
};
