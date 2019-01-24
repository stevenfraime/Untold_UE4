

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UntoldCharacter.generated.h"

class UUntoldCharacterAnimInstance;

UENUM(BlueprintType)
enum class EMeleeDirection : uint8
{
	Up, Down, Right, Left
};


UCLASS()
class UNTOLD_API AUntoldCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUntoldCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnMoveForward(float forward);
	void OnMoveRight(float right);
	void OnYaw(float yaw);
	void OnPitch(float pitch);

	void OnPrimaryPressed();
	void OnPrimaryReleased();

	FRotator lastRotator = FRotator::ZeroRotator;
	FRotator deltaRotator = FRotator::ZeroRotator;
	FRotator attackRotator = FRotator::ZeroRotator;

	UUntoldCharacterAnimInstance* animInstance = NULL;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
