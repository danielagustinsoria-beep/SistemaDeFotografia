// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DA_PhotoObject.h"
#include "PhotoActorComponent.generated.h"

class UPhotoObjectDA;

// DECLARACIÓN DEL DELEGADO: Un parámetro de tipo UDA_PhotoObject* llamado 'DatosObjeto'
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhotoRegisteredSignature, UDA_PhotoObject*, DatosObjeto);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SISTEMADEFOTOGRAFIA_API UPhotoActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPhotoActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Lista para guardar las fotos únicas
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Codice")
	TArray<TObjectPtr<UDA_PhotoObject>> PhotoCodex;

	// Instancia del delegado expuesta a Unreal
	UPROPERTY(BlueprintAssignable, Category = "Fotografia")
	FOnPhotoRegisteredSignature OnPhotoRegistered;

	void SavePhotoInCodex(TObjectPtr<UDA_PhotoObject> NewPhotoObject);
};
