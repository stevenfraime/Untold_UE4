// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemDatabaseComponent.h"

// Sets default values for this component's properties
UItemDatabaseComponent::UItemDatabaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

FItemData UItemDatabaseComponent::GetItemData(FName itemID)
{
	if (itemDatabase != NULL && !hasParsedItemDatabase)
	{
		TArray<FName> names = itemDatabase->GetRowNames();

		for (int i = 0; i < names.Num(); i++)
		{
			const FString none = "";
			FItemData* itemData = itemDatabase->FindRow<FItemData>(names[i], none, false);
			itemData->id = names[i];

			items.Add(itemData->id, *itemData);
		}

		hasParsedItemDatabase = true;
	}

	if (items.Contains(itemID))
		return items[itemID];
	return FItemData();
}