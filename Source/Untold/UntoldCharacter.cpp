

#include "UntoldCharacter.h"
#include "UntoldCharacterAnimInstance.h"

// Sets default values
AUntoldCharacter::AUntoldCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUntoldCharacter::BeginPlay()
{
	Super::BeginPlay();
	animInstance = Cast<UUntoldCharacterAnimInstance>(GetMesh()->GetAnimInstance());
}

void AUntoldCharacter::OnMoveForward(float forward)
{
	AddMovementInput(GetActorForwardVector(), forward);
}

void AUntoldCharacter::OnMoveRight(float right)
{
	AddMovementInput(GetActorRightVector(), right);
}

void AUntoldCharacter::OnYaw(float yaw)
{
	AddControllerYawInput(yaw);
}

void AUntoldCharacter::OnPitch(float pitch)
{
	AddControllerPitchInput(pitch);
}

void AUntoldCharacter::OnPrimaryPressed()
{
	if (FMath::Abs(deltaRotator.Yaw) <= 1)
	{
		if (deltaRotator.Pitch > 0)
			animInstance->StartMeleeAttack(EMeleeDirection::Down);
		else
			animInstance->StartMeleeAttack(EMeleeDirection::Up);
	}
	else if (deltaRotator.Yaw > 0)
		animInstance->StartMeleeAttack(EMeleeDirection::Left);
	else
		animInstance->StartMeleeAttack(EMeleeDirection::Right);

	attackRotator = deltaRotator;
}

void AUntoldCharacter::OnPrimaryReleased()
{
	if (FMath::Abs(attackRotator.Yaw) <= 1)
	{
		if (attackRotator.Pitch > 0)
			animInstance->ExecuteMeleeAttack(EMeleeDirection::Down);
		else
			animInstance->ExecuteMeleeAttack(EMeleeDirection::Up);
	}
	else if (attackRotator.Yaw > 0)
		animInstance->ExecuteMeleeAttack(EMeleeDirection::Left);
	else
		animInstance->ExecuteMeleeAttack(EMeleeDirection::Right);

	attackRotator = FRotator::ZeroRotator;
}

// Called every frame
void AUntoldCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	deltaRotator = lastRotator - GetControlRotation();
	lastRotator = GetControlRotation();
}

// Called to bind functionality to input
void AUntoldCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	InputComponent->BindAxis("MoveForward/Backwards", this, &AUntoldCharacter::OnMoveForward);
	InputComponent->BindAxis("MoveRight/Left", this, &AUntoldCharacter::OnMoveRight);

	InputComponent->BindAxis("LookLeft/Right", this, &AUntoldCharacter::OnYaw);
	InputComponent->BindAxis("LookUp/Down", this, &AUntoldCharacter::OnPitch);

	InputComponent->BindAction("Primary", IE_Pressed, this, &AUntoldCharacter::OnPrimaryPressed);
	InputComponent->BindAction("Primary", IE_Released, this, &AUntoldCharacter::OnPrimaryReleased);
}

