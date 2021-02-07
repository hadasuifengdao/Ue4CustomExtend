// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MovieSceneHandleShakeTrack.h"
#include "MovieSceneHandleShakeSection.h"
#include "Compilation/IMovieSceneTemplateGenerator.h"
#include "Evaluation/PersistentEvaluationData.h"
#include "Evaluation/MovieSceneEvaluationTrack.h"
#include "MovieSceneHandleShakeTemplate.h"

#define LOCTEXT_NAMESPACE "MovieSceneHandleShakeTrack"

UMovieSceneHandleShakeTrack::UMovieSceneHandleShakeTrack( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
{
}


void UMovieSceneHandleShakeTrack::PostCompile(FMovieSceneEvaluationTrack& OutTrack, const FMovieSceneTrackCompilerArgs& Args) const
{
	OutTrack.SetEvaluationPriority(GetEvaluationPriority());
}


bool UMovieSceneHandleShakeTrack::IsEmpty() const
{
	return Sections.Num() == 0;
}


void UMovieSceneHandleShakeTrack::AddSection(UMovieSceneSection& Section)
{
	Sections.Add(&Section);
}


void UMovieSceneHandleShakeTrack::RemoveSection( UMovieSceneSection& Section )
{
	Sections.Remove(&Section);
}


UMovieSceneSection* UMovieSceneHandleShakeTrack::CreateNewSection()
{
	UMovieSceneHandleShakeSection* new_section = NewObject<UMovieSceneHandleShakeSection>(this, UMovieSceneHandleShakeSection::StaticClass(), NAME_None, RF_Transactional);

	return new_section;
}


const TArray<UMovieSceneSection*>& UMovieSceneHandleShakeTrack::GetAllSections() const
{
	return Sections;
}



bool UMovieSceneHandleShakeTrack::HasSection(const UMovieSceneSection& Section) const
{
	return Sections.Contains(&Section);
}

UMovieSceneHandleShakeSection* UMovieSceneHandleShakeTrack::GetSectionByID(int id)
{
	for (UMovieSceneSection* curSection : Sections)
	{
		UMovieSceneHandleShakeSection* dialogueSection = Cast<UMovieSceneHandleShakeSection>(curSection);

		if (dialogueSection && dialogueSection->GetUniqueID() == id)
		{
			return dialogueSection;
		}
	}

	return nullptr;
}
#if WITH_EDITORONLY_DATA
FText UMovieSceneHandleShakeTrack::GetDefaultDisplayName() const
{
	return LOCTEXT("DisplayName", "HandleShake");
}
#endif

#undef LOCTEXT_NAMESPACE
