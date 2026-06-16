#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DA_PhotoObject.h"
#include "PhotoInterface.generated.h"

// Clase base de la interfaz (obligatorio en Unreal)
UINTERFACE(MinimalAPI)
class UPhotoInterface : public UInterface
{
    GENERATED_BODY()
};

// Aquí definimos las funciones que los actores deberán implementar
class SISTEMADEFOTOGRAFIA_API IPhotoInterface
{
    GENERATED_BODY()

public:
    // Esta función permite al personaje obtener el Data Asset de cualquier actor que implemente esta interfaz
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Fotografia")
    UDA_PhotoObject* GetPhotoData() const;
};