// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"

#include "PaladinHandleShakeTrackInterface.generated.h"

class UAnimSequenceBase;
class AActor;

UINTERFACE(Blueprintable, MinimalAPI)
class UPaladinHandleShakeTrackInterface : public UInterface
{
	GENERATED_BODY()
};

class PAL7_API IPaladinHandleShakeTrackInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Bubble)
	void PlayFaceAnimation(UAnimSequenceBase* FaceAnimation);

	virtual void PlayFaceAnimation_Implementation(UAnimSequenceBase* FaceAnimation);

};

