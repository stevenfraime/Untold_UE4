// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "ItemContainerComponent.generated.h"

class AItemActor;

static int EMPTY_ITEM_INDEX = -1;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Misc,
	Head,
	Torso,
	Legs,
	Gloves,
	Boots,
	Joints,
	Backpack,
	Belt,
	Weapon1H,
	Weapon2H,
	OffHand,
	Face
};

USTRUCT(BlueprintType)
struct FGridPoint
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int x = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int y = 0;

	FGridPoint() {}
	FGridPoint(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	FGridPoint operator+(const FGridPoint rhs) 
	{
		return FGridPoint(x + rhs.x, y + rhs.y);
	}
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY();

public:

	UPROPERTY(BlueprintReadOnly)
	FName id = NAME_None; //copy of the row name.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText displayName = FText::FromString("<UNDEFINED NAME>");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText displayDescription = FText::FromString("<UNDEFINED DESCRIPTION>");;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGridPoint gridSize = FGridPoint(1,1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* icon = NULL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType itemType = EItemType::Misc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isUseable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AItemActor> itemActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int maxStackSize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int weight = 1;
};

USTRUCT(BlueprintType)
struct FItemInstance
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName itemID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int stackCount = 1;
};

USTRUCT(BlueprintType)
struct FItemGrid
{
	GENERATED_BODY();

public:

	UPROPERTY()
	TArray<FItemInstance> itemTable;

	UPROPERTY()
	TArray<int> itemRefs;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInsertItem, FItemInstance, itemInstance, FGridPoint, topLeft);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoveItem, FGridPoint, topLeft);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UItemContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemContainerComponent();

	UPROPERTY(BlueprintAssignable)
	FOnInsertItem OnInsertItem;

	UPROPERTY(BlueprintAssignable)
	FOnRemoveItem OnRemoveItem;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGridPoint GridSize;

	UPROPERTY(EditDefaultsOnly)
	int maxWeight = 100;

	UPROPERTY(ReplicatedUsing=OnRep_ItemGrid)
	FItemGrid itemGrid;
	FItemGrid oldItemGrid;

	int gridCount = 0;

	UFUNCTION(BlueprintPure)
	int GetGridIndex(FGridPoint point);

	UFUNCTION(BlueprintPure)
	FGridPoint GetGridPoint(int index);

	UFUNCTION(BlueprintPure)
	TArray<int> GetRequiredIndexes(FGridPoint topLeft, FGridPoint size);

	UFUNCTION(BlueprintPure)
	bool CanFit(FGridPoint topLeft, FGridPoint size);

	UFUNCTION()
	void OnRep_ItemGrid();

public:

	UFUNCTION(BlueprintCallable)
	bool AddItem(FItemInstance item);

	UFUNCTION(BlueprintCallable)
	bool InsertItem(FItemInstance item, FGridPoint gridPoint);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(FGridPoint gridPoint);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemByID(FName itemID);

	UFUNCTION(BlueprintCallable)
	bool TransferItem(FGridPoint sourcePoint, UItemContainerComponent* targetContainer, FGridPoint targetPoint);

	UFUNCTION(BlueprintPure)
	FItemInstance GetItem(FGridPoint point);

	UFUNCTION(BlueprintPure)
	TArray<FItemInstance> GetItems();
};
