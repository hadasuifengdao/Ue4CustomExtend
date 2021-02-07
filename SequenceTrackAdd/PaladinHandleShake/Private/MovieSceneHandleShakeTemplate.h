// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Evaluation/PersistentEvaluationData.h"
#include "Evaluation/MovieSceneEvaluationTrack.h"
#include "Evaluation/MovieSceneEvalTemplate.h"
#include "CustomTracks/PaladinHandleShake/MovieSceneHandleShakeSection.h"
#include "MovieSceneHandleShakeTemplate.generated.h"

USTRUCT()
struct FMovieSceneHandleShakeSectionTemplate: public FMovieSceneEvalTemplate
{
	GENERATED_BODY()

	FMovieSceneHandleShakeSectionTemplate();

	FMovieSceneHandleShakeSectionTemplate(const UMovieSceneHandleShakeSection& Section);

private:
	virtual UScriptStruct& GetScriptStructImpl() const override
	{
		return *StaticStruct();
	}

	virtual void SetupOverrides() override
	{
		// EnableOverrides(RequiresInitializeFlag | RequiresSetupFlag);
		EnableOverrides(RequiresSetupFlag | RequiresTearDownFlag); // Setup只在轨道开头，TearDown只在轨道结束
	}

	virtual void Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;
	virtual void Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const override;
	virtual void TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;
public:
	UPROPERTY()
	class UForceFeedbackEffect* HandleForceFeedbackEffect;

	UPROPERTY()
	float DurationScale = -1.f;

	UPROPERTY()
	float VerticalScale = 1.f;
};


USTRUCT()
struct FMovieSceneHandleShakeSharedTrack //: public FMovieSceneEvaluationTrack
{
	GENERATED_BODY()

    static FSharedPersistentDataKey GetSharedDataKey();
};

//lint -restore