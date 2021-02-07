// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "MovieSceneNameableTrack.h"
#include "Tracks/MovieSceneSpawnTrack.h"
#include "Tracks/MovieScenePropertyTrack.h"
#include "MovieSceneHandleShakeTrack.generated.h"

struct FMovieSceneEvaluationTrack;
class UMovieSceneHandleShakeSection;

UCLASS()
class PAL7_API UMovieSceneHandleShakeTrack : public UMovieSceneNameableTrack
{
	GENERATED_UCLASS_BODY()

public:

	static uint16 GetEvaluationPriority() { return UMovieSceneSpawnTrack::GetEvaluationPriority() + 120; }

	/** UMovieSceneTrack interface */
	virtual void PostCompile(FMovieSceneEvaluationTrack& Track, const FMovieSceneTrackCompilerArgs& Args) const override;
	virtual bool IsEmpty() const override;
	virtual void AddSection(UMovieSceneSection& Section) override;
	virtual void RemoveSection( UMovieSceneSection& Section ) override;
	virtual UMovieSceneSection* CreateNewSection() override;
	virtual const TArray<UMovieSceneSection*>& GetAllSections() const override;
//	virtual TRange<float> GetSectionBoundaries() const override;
	virtual bool HasSection(const UMovieSceneSection& Section) const override;
	virtual bool SupportsMultipleRows() const override { return true; }
	/**
 * Get a raw compiled copy of this track with no additional shared tracks or compiler parameters
 */
	//MOVIESCENE_API virtual void GenerateTemplate(const FMovieSceneTrackCompilerArgs& Args) const override;

#if WITH_EDITORONLY_DATA
	virtual FText GetDefaultDisplayName() const override;
#endif

public:
	UMovieSceneHandleShakeSection* GetSectionByID(int id);

private:
	UPROPERTY()
	TArray<UMovieSceneSection*> Sections;

};
