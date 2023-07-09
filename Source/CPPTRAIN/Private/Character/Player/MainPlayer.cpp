// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/MainPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h" // UInputComponent 用于行为和输入之间的绑定
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	// 设置悬臂旋转
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 400.0f;
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	// 设置相机不旋转
	FollowCamera->bUsePawnControlRotation = false;
	// 设置角色与胶囊体底部对齐 （因为 capsulecomponent是个private的所以要get）
	GetCapsuleComponent()->SetCapsuleSize(40.0f, 88.0f);

	this->bUseControllerRotationYaw = false;
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationRoll = false;

	// 设置角色移向相机正方向，其中rotationrate控制插值
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	
	// 设置角色jump的速率，其影响跳跃的高度。aircontrol影响的是在空中的可控性
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
	// 动作映射
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// 轴映射
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);
	
	// 直接使用 ACharacter 类预先提供好的
	PlayerInputComponent->BindAxis("Turn", this, &AMainPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainPlayer::LookUp);

	// 绑定键盘轴映射
	PlayerInputComponent->BindAxis("TurnRate", this, &AMainPlayer::TurnRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainPlayer::LookUpRate);

}

void AMainPlayer::Jump()
{
	Super::Jump();
}

void AMainPlayer::MoveForward(float Value)
{
	FRotator Rotation = Controller->GetControlRotation(); // 获取到controller的旋转属性
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(direction, Value);
}

void AMainPlayer::MoveRight(float Value)
{
	FRotator Rotation = Controller->GetControlRotation(); // 获取到controller的旋转属性
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
		FRotator PitchRotation = GetControlRotation(); // 如果一下拉得太大，会跳到可控区域外，造成卡死
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
	// 目的是为了平衡不同刷新率之间单位时间移动距离相等
	float Value = Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds();
	if (Value != 0) {
		AddControllerYawInput(Value);
	}
}

void AMainPlayer::LookUpRate(float Rate)
{
	float Value = Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds();
	if (Value != 0.0f) {
		FRotator PitchRotation = GetControlRotation(); // 如果一下拉得太大，会跳到可控区域外，造成卡死
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
  