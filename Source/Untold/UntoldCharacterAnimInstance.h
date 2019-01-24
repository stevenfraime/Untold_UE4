

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UntoldCharacter.h"
#include "UntoldCharacterAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class UNTOLD_API UUntoldCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void StartMeleeAttack(EMeleeDirection meleeDirection);

	UFUNCTION(BlueprintImplementableEvent)
	void ExecuteMeleeAttack(EMeleeDirection meleeDirection);

	UPROPERTY(BlueprintReadOnly)
	FVector normalizedVelocity = FVector::ZeroVector;
};
