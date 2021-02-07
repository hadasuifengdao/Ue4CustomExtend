// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#include "MovieSceneHandleShakeSection.h"
#include "Evaluation/MovieSceneEvalTemplate.h"
#include "MovieSceneHandleShakeTemplate.h"
#include "GameFramework/ForceFeedbackEffect.h"

UMovieSceneHandleShakeSection::UMovieSceneHandleShakeSection( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
{
}

FMovieSceneEvalTemplatePtr UMovieSceneHandleShakeSection::GenerateTemplate() const
{
	return FMovieSceneHandleShakeSectionTemplate(*this);
}


TOptional<TRange<FFrameNumber>> UMovieSceneHandleShakeSection::GetAutoSizeRange() const 
{	
	float ForceFeedBackTime = 1.0f;
	if (HandleForceFeedbackEffect)
	{
		HandleForceFeedbackEffect->DurationScale = DurationScale;
		ForceFeedBackTime = (HandleForceFeedbackEffect->Duration) * (HandleForceFeedbackEffect->DurationScale);
	}
	FFrameRate FrameRate = GetTypedOuter<UMovieScene>()->GetTickResolution();
	FFrameTime DurationToUse = ForceFeedBackTime * FrameRate;
	return TRange<FFrameNumber>(GetInclusiveStartFrame(), GetInclusiveStartFrame() + DurationToUse.FrameNumber);
}

#if WITH_EDITOR
void UMovieSceneHandleShakeSection::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	TOptional<TRange<FFrameNumber>> value = GetAutoSizeRange();
	if (value.IsSet())
	{
		SetRange(value.GetValue());
	}
}
#endif


