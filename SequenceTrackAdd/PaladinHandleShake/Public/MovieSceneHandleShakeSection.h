// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Curves/KeyHandle.h"
#include "MovieSceneSection.h"
#include "MovieSceneHandleShakeSection.generated.h"

class UAnimSequenceBase;
class UForceFeedbackEffect;

UCLASS()
class PAL7_API UMovieSceneHandleShakeSection : public UMovieSceneSection
{
   GENERATED_UCLASS_BODY()

public:
	void SetForceFeedBackEffect(UForceFeedbackEffect* InForceFeedbackEffect)		{ HandleForceFeedbackEffect = InForceFeedbackEffect; }
	FORCEINLINE UForceFeedbackEffect* GetForceFeedBackEffect()	const	{ return HandleForceFeedbackEffect; }

	void SetDurationScale(float InDurationScale) { DurationScale = InDurationScale; }
	FORCEINLINE float GetDurationScale()	const { return DurationScale; }

	void SetVerticalScale(float InVerticalScale) { VerticalScale = InVerticalScale; }
	FORCEINLINE float GetVerticalScale()	const { return VerticalScale; }
public:

	//~ UMovieSceneSection interface
	virtual FMovieSceneEvalTemplatePtr GenerateTemplate() const override;
	
private:
	UPROPERTY(EditAnywhere, Category = HandleShake)
	class UForceFeedbackEffect* HandleForceFeedbackEffect;

	UPROPERTY(EditAnywhere, Category = HandleShake)
	float DurationScale = 1.f;

	UPROPERTY(EditAnywhere, Category = HandleShake)
	float VerticalScale = 1.f;

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
	virtual TOptional<TRange<FFrameNumber>> GetAutoSizeRange()const override;
};
