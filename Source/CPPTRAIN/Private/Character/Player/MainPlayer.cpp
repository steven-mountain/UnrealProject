// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/MainPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h" // UInputComponent ������Ϊ������֮��İ�
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	// ����������ת
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 400.0f;
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	// �����������ת
	FollowCamera->bUsePawnControlRotation = false;
	// ���ý�ɫ�뽺����ײ����� ����Ϊ capsulecomponent�Ǹ�private������Ҫget��
	GetCapsuleComponent()->SetCapsuleSize(40.0f, 88.0f);

	this->bUseControllerRotationYaw = false;
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationRoll = false;

	// ���ý�ɫ�����������������rotationrate���Ʋ�ֵ
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	
	// ���ý�ɫjump�����ʣ���Ӱ����Ծ�ĸ߶ȡ�aircontrolӰ������ڿ��еĿɿ���
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.15f;
	
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	// ����ӳ��
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// ��ӳ��
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);
	
	// ֱ��ʹ�� ACharacter ��Ԥ���ṩ�õ�
	PlayerInputComponent->BindAxis("Turn", this, &AMainPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainPlayer::LookUp);

	// �󶨼�����ӳ��
	PlayerInputComponent->BindAxis("TurnRate", this, &AMainPlayer::TurnRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainPlayer::LookUpRate);

}

void AMainPlayer::Jump()
{
	Super::Jump();
}

void AMainPlayer::MoveForward(float Value)
{
	FRotator Rotation = Controller->GetControlRotation(); // ��ȡ��controller����ת����
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(direction, Value);
}

void AMainPlayer::MoveRight(float Value)
{
	FRotator Rotation = Controller->GetControlRotation(); // ��ȡ��controller����ת����
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(direction, Value);
}

void AMainPlayer::Turn(float Value)
{
	if (Value != 0) {
		AddControllerYawInput(Value);
	}
}

void AMainPlayer::LookUp(float Value)
{
	/*FRotator PitchRotation = GetControlRotation();*/
	if (Value != 0.0f) {
		FRotator PitchRotation = GetControlRotation(); // ���һ������̫�󣬻������ɿ������⣬��ɿ���
		if ((PitchRotation.Pitch > 45.0f && PitchRotation.Pitch < 180.0f && Value > 0.0f)||
			(PitchRotation.Pitch > 180.0f && PitchRotation.Pitch < 270.0f && Value < 0.0f))
			return;
		/*if ((NewAngle < 40.0f && NewAngle >= 0.0f) ||
			(NewAngle > 270.0f && NewAngle <= 360.0f)) {
			AddControllerPitchInput(-Value);
		}*/
		AddControllerPitchInput(-Value);
	}
}

void AMainPlayer::TurnRate(float Rate)
{
	/*FRotator PitchRotation = GetControlRotation();*/
	// Ŀ����Ϊ��ƽ�ⲻͬˢ����֮�䵥λʱ���ƶ��������
	float Value = Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds();
	if (Value != 0) {
		AddControllerYawInput(Value);
	}
}

void AMainPlayer::LookUpRate(float Rate)
{
	float Value = Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds();
	if (Value != 0.0f) {
		FRotator PitchRotation = GetControlRotation(); // ���һ������̫�󣬻������ɿ������⣬��ɿ���
		if ((PitchRotation.Pitch > 45.0f && PitchRotation.Pitch < 180.0f && Value > 0.0f) ||
			(PitchRotation.Pitch > 180.0f && PitchRotation.Pitch < 270.0f && Value < 0.0f))
			return;
		/*if ((NewAngle < 40.0f && NewAngle >= 0.0f) ||
			(NewAngle > 270.0f && NewAngle <= 360.0f)) {
			AddControllerPitchInput(-Value);
		}*/
		AddControllerPitchInput(-Value);
	}
}
  