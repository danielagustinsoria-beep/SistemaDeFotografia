#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_PhotoObject.generated.h"

UCLASS(BlueprintType)
class SISTEMADEFOTOGRAFIA_API UDA_PhotoObject : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fotografia")
    FString NombreObjeto;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fotografia", meta = (MultiLine = true))
    FString DescripcionCodice;
};