// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemContainerComponent.h"
#include "ItemDatabaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UItemDatabaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemDatabaseComponent();

	UPROPERTY(EditDefaultsOnly)
	UDataTable* itemDatabase = NULL;
	bool hasParsedItemDatabase = false;

	TMap<FName, FItemData> items;
	FItemData GetItemData(FName itemID);


};
