// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/MainPlayerAnimInstance.h"
#include "Character/Player/MainPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMainPlayerAnimInstance::NativeInitializeAnimation()
{
	// ��Ϊ���ص��� pawn����character�̳���pawn��������Ҫ��pawnת����character
	MainPlayerRef = Cast<AMainPlayer>(TryGetPawnOwner());
}

void UMainPlayerAnimInstance::UpdateAnimationProperties()
{
	if (MainPlayerRef == nullptr) {
		MainPlayerRef = Cast<AMainPlayer>(TryGetPawnOwner());
	}
	if (MainPlayerRef) {
		FVector SpeedVector = MainPlayerRef->GetVelocity();
		FVector PlaneVector(SpeedVector.X, SpeedVector.Y, 0.0f);
		Speed = PlaneVector.Size();
		bIsInAir = MainPlayerRef->GetMovementComponent()->IsFalling();
	}
}
