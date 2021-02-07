// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#include "HandleShakeSection.h"
#include "MovieSceneHandleShakeSection.h"
#include "Misc/PackageName.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "SequencerSectionPainter.h"
#include "SDropTarget.h"
#include "DragAndDrop/LevelDragDropOp.h"
#include "ScopedTransaction.h"

FHandleShakeSection::FHandleShakeSection( UMovieSceneHandleShakeSection& InSectionObject )
	: SectionObject( InSectionObject )
{
}

UMovieSceneSection* FHandleShakeSection::GetSectionObject()
{
	return &SectionObject;
}

TSharedRef<SWidget> FHandleShakeSection::GenerateSectionWidget()
{
	return
		SNew( SDropTarget )
		.OnAllowDrop( this, &FHandleShakeSection::OnAllowDrop )
		.OnDrop( this, &FHandleShakeSection::OnDrop )
		.Content()
		[
			SNew( SBorder )
			.BorderBackgroundColor( this, &FHandleShakeSection::GetBackgroundColor )
			.BorderImage( FCoreStyle::Get().GetBrush( "WhiteBrush" ) )
			[
				SNew( STextBlock )
				.Text( this, &FHandleShakeSection::GetVisibilityText )
				.ToolTipText( this, &FHandleShakeSection::GetVisibilityToolTip )
			]
		];
}


int32 FHandleShakeSection::OnPaintSection( FSequencerSectionPainter& InPainter ) const
{
	return InPainter.PaintSectionBackground();
}


FText FHandleShakeSection::GetSectionTitle() const
{
	return FText();
}


FSlateColor FHandleShakeSection::GetBackgroundColor() const
{
	if(IsTitleInfo())
	{
		return FSlateColor(FLinearColor::Black.Desaturate(.5f));
	}

	return FSlateColor(FLinearColor::Green.Desaturate(.5f));
}


FText FHandleShakeSection::GetVisibilityText() const
{
	return  FText();
	//if (IsTitleInfo())
	//{
	//	//return SectionObject.GetTitleInfo();
	//}

	//return SectionObject.GetHandleShakeInfo();
	//return FText::Format(NSLOCTEXT("HandleShakeTrackEditor","HandleShakeSectionText", "{0}:{1}"),SectionObject.GetHandleShakeSpeakerName(),SectionObject.GetHandleShakeInfo());
}

FText FHandleShakeSection::GetVisibilityToolTip() const
{
	//return FText::Format(NSLOCTEXT("HandleShakeTrackEditor", "HandleShakeSectionText", "{0} {1}:\n{2}"), SectionObject.GetTitleInfo(), SectionObject.GetHandleShakeSpeakerName(), SectionObject.GetHandleShakeInfo());
	return  FText();
}


bool FHandleShakeSection::OnAllowDrop( TSharedPtr<FDragDropOperation> DragDropOperation )
{
	return false;
}


FReply FHandleShakeSection::OnDrop( TSharedPtr<FDragDropOperation> DragDropOperation )
{
	return FReply::Unhandled();
}

bool FHandleShakeSection::IsTitleInfo()const
{
	//return !SectionObject.GetTitleInfo().IsEmptyOrWhitespace();
	return false;
}

