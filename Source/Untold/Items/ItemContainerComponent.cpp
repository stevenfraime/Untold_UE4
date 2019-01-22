// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemContainerComponent.h"
#include "ItemActor.h"
#include "Engine/World.h"
#include "ItemFunctionLibrary.h"
#include "UnrealNetwork.h"

// Sets default values for this component's properties
UItemContainerComponent::UItemContainerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bReplicates = true;

	GridSize = FGridPoint(12, 32);
}

void UItemContainerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UItemContainerComponent, itemGrid);
}

// Called when the game starts
void UItemContainerComponent::BeginPlay()
{
	Super::BeginPlay();
	gridCount = GridSize.x * GridSize.y;

	itemGrid.itemTable.SetNum(gridCount);
	itemGrid.itemRefs.SetNum(gridCount);
	for (int i = 0; i < gridCount; i++)
		itemGrid.itemRefs[i] = EMPTY_ITEM_INDEX;

	oldItemGrid = itemGrid;
}

int UItemContainerComponent::GetGridIndex(FGridPoint point)
{
	int index = GridSize.x * point.y + point.x;
	return index;
}

FGridPoint UItemContainerComponent::GetGridPoint(int index)
{
	FGridPoint point;

	point.y = index / GridSize.x;
	point.x = index % GridSize.x; 

	return point;
}

TArray<int> UItemContainerComponent::GetRequiredIndexes(FGridPoint topLeft, FGridPoint size)
{
	TArray<int> indexes;
	for (int x = 0; x < size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
			FGridPoint checkPoint = topLeft + FGridPoint(x, y);
			int index = GetGridIndex(checkPoint);
			indexes.Add(index);
		}
	}

	return indexes;
}

// need to add an optional ignore itemIndex parameter, that allows overlapping of moved items.
bool UItemContainerComponent::CanFit(FGridPoint topLeft, FGridPoint size)
{
	int topLeftIndex = GetGridIndex(topLeft);
	int itemIndex = itemGrid.itemRefs[topLeftIndex];

	TArray<int> indexes = GetRequiredIndexes(topLeft, size);

	if (topLeft.x + size.x > GridSize.x)
		return false;
	if (topLeft.y + size.y > GridSize.y)
		return false;

	for (int i = 0; i < indexes.Num(); i++)
	{
		int checkIndex = itemGrid.itemRefs[indexes[i]];
		if (checkIndex != EMPTY_ITEM_INDEX)
			return false;
	}

	return true;
}

void UItemContainerComponent::OnRep_ItemGrid()
{
	if (oldItemGrid.itemTable.Num() == 0)
		oldItemGrid = itemGrid;

	for (int i = 0; i < itemGrid.itemTable.Num(); i++)
	{
		FGridPoint point = GetGridPoint(i);
		if (oldItemGrid.itemTable[i].itemID != itemGrid.itemTable[i].itemID)
		{
			if (itemGrid.itemTable[i].itemID != NAME_None)
				OnInsertItem.Broadcast(itemGrid.itemTable[i], point);
			else
				OnRemoveItem.Broadcast(point);
		}
	}

	oldItemGrid = itemGrid;
}

bool UItemContainerComponent::AddItem(FItemInstance item)
{
	if (!GetWorld()->IsServer()) return false;

	// find space, if there is space, do insert.
	FItemData itemData = UItemFunctionLibrary::GetItemData(item.itemID, this);

	bool foundSpace = false;
	FGridPoint point;

	for (int i = 0; i < gridCount; i++)
	{
		point = GetGridPoint(i);
		if (CanFit(point, itemData.gridSize))
		{
			foundSpace = true;
			break;
		}
	}
	if (!foundSpace)
		return false;

	return InsertItem(item, point);
}

bool UItemContainerComponent::InsertItem(FItemInstance item, FGridPoint gridPoint)
{
	if (!GetWorld()->IsServer()) return false;

	FItemData itemData = UItemFunctionLibrary::GetItemData(item.itemID, this);
	if (!CanFit(gridPoint, itemData.gridSize)) // always validate space!
		return false;

	int itemIndex = GetGridIndex(gridPoint);
	itemGrid.itemTable[itemIndex] = item;
	
	// add the items to the grid.
	TArray<int> indexes = GetRequiredIndexes(gridPoint, itemData.gridSize);
	for (int i = 0; i < indexes.Num(); i++)
		itemGrid.itemRefs[indexes[i]] = itemIndex;

	itemGrid = itemGrid;
	OnRep_ItemGrid();
	return true;
}

bool UItemContainerComponent::RemoveItemByID(FName itemID)
{
	if (!GetWorld()->IsServer()) return false;

	for (int i = 0; i < gridCount; i++)
	{
		if (itemGrid.itemTable[i].itemID == itemID)
		{
			FGridPoint point = GetGridPoint(i);
			return RemoveItem(point);
		}
	}

	return false;
}

bool UItemContainerComponent::RemoveItem(FGridPoint gridPoint)
{
	if (!GetWorld()->IsServer()) return false;

	int removeIndex = GetGridIndex(gridPoint);
	if (itemGrid.itemRefs[removeIndex] == EMPTY_ITEM_INDEX)
		return false;
	
	// using the remove index, we figure out the item index (top left index) using the item grid.
	int itemIndex = itemGrid.itemRefs[removeIndex];
	if (itemGrid.itemTable[itemIndex].itemID == NAME_None) // there was an index, but it wasnt referencing a real item.
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid item table reference in the item grid."));
		return false;
	}

	// get the item instance out of the item table using the item index.
	FItemInstance itemInstance = itemGrid.itemTable[itemIndex];
	FItemData itemData = UItemFunctionLibrary::GetItemData(itemInstance.itemID, this);

	FGridPoint topLeft = GetGridPoint(itemIndex); // using the item table index, figure out the grid point.
	TArray<int> indexes = GetRequiredIndexes(topLeft, itemData.gridSize);
	for (int i = 0; i < indexes.Num(); i++)
		itemGrid.itemRefs[indexes[i]] = EMPTY_ITEM_INDEX;

	itemGrid.itemTable[itemIndex] = FItemInstance(); // clear out the item.

	FGridPoint removedTopLeft = GetGridPoint(itemIndex);

	itemGrid = itemGrid;
	OnRep_ItemGrid();
	return true;
}

bool UItemContainerComponent::TransferItem(FGridPoint sourcePoint, UItemContainerComponent* targetContainer, FGridPoint targetPoint)
{
	if (!GetWorld()->IsServer()) return false;

	int sourceIndex = GetGridIndex(sourcePoint);
	if (itemGrid.itemRefs[sourceIndex] == EMPTY_ITEM_INDEX)
		return false;

	int itemIndex = itemGrid.itemRefs[sourceIndex];
	if (itemGrid.itemTable[itemIndex].itemID == NAME_None) // can probably simplify these checks.
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid item table reference in the item grid."));
		return false;
	}
	FItemInstance item = itemGrid.itemTable[itemIndex];

	if (!targetContainer->InsertItem(item, targetPoint))
		return false;
	

	targetContainer->RemoveItem(sourcePoint);
	return true;
}

FItemInstance UItemContainerComponent::GetItem(FGridPoint point)
{
	int index = GetGridIndex(point);
	return itemGrid.itemTable[index];
}

TArray<FItemInstance> UItemContainerComponent::GetItems()
{
	return itemGrid.itemTable;
}
