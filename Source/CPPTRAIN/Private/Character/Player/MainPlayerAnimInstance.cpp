// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/MainPlayerAnimInstance.h"
#include "Character/Player/MainPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMainPlayerAnimInstance::NativeInitializeAnimation()
{
	// 因为返回的是 pawn，而character继承自pawn，所以需要将pawn转换成character
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
