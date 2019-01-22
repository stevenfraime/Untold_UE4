// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemDatabaseComponent.h"
#include "ItemFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UItemFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintPure, Category = "Item", meta = (WorldContext = "WorldContextObject"))
	static FItemData GetItemData(FName itemID, const UObject* WorldContextObject);

};
