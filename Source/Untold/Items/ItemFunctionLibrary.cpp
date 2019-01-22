// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemFunctionLibrary.h"
#include "GameFramework/GameState.h"
#include "Engine/World.h"

FItemData UItemFunctionLibrary::GetItemData(FName itemID, const UObject* WorldContextObject)
{
	if (WorldContextObject == NULL || WorldContextObject->GetWorld() == NULL) return FItemData();
	AGameStateBase* gameState = WorldContextObject->GetWorld()->GetGameState();
	if (gameState == NULL)
		return FItemData();

	UItemDatabaseComponent* itemDatabase = Cast<UItemDatabaseComponent>(gameState->GetComponentByClass(UItemDatabaseComponent::StaticClass()));
	if (itemDatabase == NULL)
		return FItemData();

	return itemDatabase->GetItemData(itemID);
}
