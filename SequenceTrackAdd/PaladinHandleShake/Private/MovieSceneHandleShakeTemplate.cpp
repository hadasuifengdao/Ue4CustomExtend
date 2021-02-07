// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#include "MovieSceneHandleShakeTemplate.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Misc/PackageName.h"
#include "MovieSceneEvaluation.h"
#include "IMovieScenePlayer.h"
#include "GameFramework/Actor.h"
#include "Animation/AnimSequenceBase.h"
#include "PaladinPlayerController.h"
#include "PaladinSequenceBlueprintLibrary.h"
#include "PaladinSequenceManager.h"
#include "PaladinBlueprintLibrary.h"
#include "CustomTracks/PaladinHandleShake/PaladinHandleShakeTrackInterface.h"
#include "PaladinBlueprintLibrary.h"
#if WITH_EDITOR
#include "PaladinGlobals.h"
#include "GameFramework/ForceFeedbackEffect.h"
#endif

struct FHandleShakeSharedTrackData : public IPersistentEvaluationData
{
	FHandleShakeSharedTrackData()
	{}

	~FHandleShakeSharedTrackData()
	{
	}

private:
	UForceFeedbackEffect* HandleForceFeedbackEffect= nullptr;
	float DurationScale = 1.f;
	float VerticalScale = 1.f;
public:

	void SetInfo(UForceFeedbackEffect* InHandleForceFeedbackEffect,float InDurationScale,float InVerticalScale)
	{
		HandleForceFeedbackEffect = InHandleForceFeedbackEffect;
		DurationScale = InDurationScale;
		VerticalScale = InVerticalScale;
	}


	void Apply(IMovieScenePlayer& Player)
	{
#if WITH_EDITOR
		UWorld* curWorld = nullptr;
		if (Player.GetPlaybackContext())
		{
			curWorld = Player.GetPlaybackContext()->GetWorld();
		}
		if (GIsEditor && !curWorld->IsPlayInEditor())
		{
			UE_LOG(LogTemp, Log, TEXT("ApplySucced"));
			if (HandleForceFeedbackEffect)
			{
				UE_LOG(LogTemp, Log, TEXT("HandleForceFeedbackEffect"));
				HandleForceFeedbackEffect->DurationScale = DurationScale;
				HandleForceFeedbackEffect->ValueScale = VerticalScale;
			}
			PlayEffect(HandleForceFeedbackEffect);
		}
#endif
 		UPaladinGlobalSpecialEffectManagerComponent* SpecialEffectComponent = UPaladinBlueprintLibrary::GetPaladinGlobalEffectManager();
		if (SpecialEffectComponent)
		{
			APaladinPlayerController* PLC = Cast<APaladinPlayerController>(UPaladinBlueprintLibrary::GetPalPlayerController());
			if (PLC)
			{
				SpecialEffectComponent->HandleShakeManager->PlayHandleShake(HandleForceFeedbackEffect, PLC, false, DurationScale, VerticalScale);
			}
		}
	}
	void clear(IMovieScenePlayer& Player)
	{
#if WITH_EDITOR
		UWorld* curWorld = nullptr;
		if (Player.GetPlaybackContext())
		{
			curWorld = Player.GetPlaybackContext()->GetWorld();
		}
		if (GIsEditor && !curWorld->IsPlayInEditor())
		{
			PlayEffect(nullptr);
		}
#endif
		APaladinPlayerController* PLC = Cast<APaladinPlayerController>(UPaladinBlueprintLibrary::GetPalPlayerController());
		if (PLC)
		{
			PLC->ClientStopForceFeedback(HandleForceFeedbackEffect, FName());
		}
	}


#if WITH_EDITOR
private:
	FPaladinPreviewForceFeedbackEffect PaladinPreviewForceFeedbackEffect;

	void PlayEffect(UForceFeedbackEffect* Effect)
	{
		if (Effect)
		{
			PaladinPreviewForceFeedbackEffect.ForceFeedbackEffect = Effect;
			PaladinPreviewForceFeedbackEffect.PlayTime = 0.f;
		}
		else
		{
			PaladinPreviewForceFeedbackEffect.ForceFeedbackEffect = nullptr;

			IInputInterface* InputInterface = FSlateApplication::Get().GetInputInterface();
			if (InputInterface)
			{
				InputInterface->SetForceFeedbackChannelValues(0, FForceFeedbackValues());
			}
		}
	}
#endif
};


struct FHandleShakeExecutionToken : public IMovieSceneExecutionToken
{

	FHandleShakeExecutionToken()
	{
	}

    void Execute(const FMovieSceneContext& Context, const FMovieSceneEvaluationOperand& Operand, FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player)
	{
	}

public:
	UForceFeedbackEffect* HandleForceFeedbackEffect = nullptr;
	float VerticalScale = 1.f;
};



FMovieSceneHandleShakeSectionTemplate::FMovieSceneHandleShakeSectionTemplate()
{
	//bPausePlayer = false;
}

FMovieSceneHandleShakeSectionTemplate::FMovieSceneHandleShakeSectionTemplate(const UMovieSceneHandleShakeSection& InSection)
{
	HandleForceFeedbackEffect = InSection.GetForceFeedBackEffect();
	DurationScale = InSection.GetDurationScale();
	VerticalScale = InSection.GetVerticalScale();
}

void FMovieSceneHandleShakeSectionTemplate::Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const
{
	FHandleShakeSharedTrackData& TrackData = PersistentData.GetOrAdd<FHandleShakeSharedTrackData>(FMovieSceneHandleShakeSharedTrack::GetSharedDataKey());

	TrackData.SetInfo(HandleForceFeedbackEffect, DurationScale,VerticalScale);
	TrackData.Apply(Player);
}
void FMovieSceneHandleShakeSectionTemplate::Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const
{
	FMovieSceneSharedDataId DataID = FMovieSceneHandleShakeSharedTrack::GetSharedDataKey().UniqueId;
	const FHandleShakeSharedTrackData* TrackData = PersistentData.Find<FHandleShakeSharedTrackData>(FMovieSceneHandleShakeSharedTrack::GetSharedDataKey());

	if (TrackData && !ExecutionTokens.FindShared(DataID))
	{
		FHandleShakeExecutionToken Token = FHandleShakeExecutionToken();
		Token.HandleForceFeedbackEffect = HandleForceFeedbackEffect;
		Token.VerticalScale = VerticalScale;
		if (GEngine && Context.GetStatus() != EMovieScenePlayerStatus::Jumping) {
			ExecutionTokens.Add(Token);
		}
	}
	
}

void FMovieSceneHandleShakeSectionTemplate::TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const
{
	FHandleShakeSharedTrackData& TrackData = PersistentData.GetOrAdd<FHandleShakeSharedTrackData>(FMovieSceneHandleShakeSharedTrack::GetSharedDataKey());
	TrackData.clear(Player);
}


FSharedPersistentDataKey FMovieSceneHandleShakeSharedTrack::GetSharedDataKey()
{
	static FMovieSceneSharedDataId DataId(FMovieSceneSharedDataId::Allocate());
	return FSharedPersistentDataKey(DataId, FMovieSceneEvaluationOperand());
}


