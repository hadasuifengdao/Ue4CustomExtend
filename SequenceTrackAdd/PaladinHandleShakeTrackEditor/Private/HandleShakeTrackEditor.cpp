#include "HandleShakeTrackEditor.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "EditorStyleSet.h"
#include "SequencerUtilities.h"
#include "MovieSceneHandleShakeTrack.h"
#include "MovieSceneHandleShakeSection.h"
#include "HandleShakeSection.h"
#include "IDetailsView.h"
#include "ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailCustomization.h"
#include "DetailLayoutBuilder.h"

//#include "MovieSceneHandleShakeSkipSection.h"
//#include "MovieSceneHandleShakeSkipTrack.h"

#include "MovieSceneAudioTrack.h"
#include "Sound/SoundBase.h"
#include "MovieSceneAtomTrack.h"
#include "MovieSceneAtomSection.h"
#include "MovieSceneSkeletalAnimationTrack.h"
#include "MovieSceneSkeletalAnimationSection.h"

//DECLARE_LOG_CATEGORY_EXTERN(LogHandleShake, Log, All);

#define LOCTEXT_NAMESPACE "HandleShakeTrackEditor.h"

FHandleShakeTrackEditor::FHandleShakeTrackEditor( TSharedRef<ISequencer> InSequencer )
	: FMovieSceneTrackEditor( InSequencer ) 
{ }


TSharedRef<ISequencerTrackEditor> FHandleShakeTrackEditor::CreateTrackEditor( TSharedRef<ISequencer> InSequencer )
{
	return MakeShareable( new FHandleShakeTrackEditor( InSequencer ) );
}


void FHandleShakeTrackEditor::ImportDataFromDataTable(UMovieSceneTrack* Track)
{
}

bool FHandleShakeTrackEditor::SupportsType( TSubclassOf<UMovieSceneTrack> Type ) const
{
	return Type == UMovieSceneHandleShakeTrack::StaticClass();
}

const FSlateBrush* FHandleShakeTrackEditor::GetIconBrush() const
{
	return FEditorStyle::GetBrush("Sequencer.Tracks.LevelVisibility");
}


TSharedRef<ISequencerSection> FHandleShakeTrackEditor::MakeSectionInterface( UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding )
{
	UMovieSceneHandleShakeSection* HandleShakeSection = Cast<UMovieSceneHandleShakeSection>(&SectionObject);
	check( SupportsType( SectionObject.GetOuter()->GetClass() ) && HandleShakeSection != nullptr );

	return MakeShareable( new FHandleShakeSection( *HandleShakeSection) );
}

void FHandleShakeTrackEditor::BuildAddTrackMenu( FMenuBuilder& MenuBuilder )
{
	UMovieSceneSequence* RootMovieSceneSequence = GetSequencer()->GetRootMovieSceneSequence();

	if ( ( RootMovieSceneSequence == nullptr ) || ( RootMovieSceneSequence->GetClass()->GetName() != TEXT( "LevelSequence" ) ) )
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("PaladinHandleShakeTrackEditorModule BuildAddTrackMenu"));

	MenuBuilder.AddMenuEntry(
		LOCTEXT("AddHandleShakeTrack", "HandleShake" ),
		LOCTEXT("AddHandleShakeToolTip", "Pal7 HandleShake Track." ),
		FSlateIcon(FEditorStyle::GetStyleSetName(), "Sequencer.Tracks.LevelVisibility"),
		FUIAction( FExecuteAction::CreateRaw( this, &FHandleShakeTrackEditor::OnAddTrack ) ) );
}


TSharedPtr<SWidget> FHandleShakeTrackEditor::BuildOutlinerEditWidget( const FGuid& ObjectBinding, UMovieSceneTrack* Track, const FBuildEditWidgetParams& Params )
{
	// Create a container edit box
	return FSequencerUtilities::MakeAddButton( 
		LOCTEXT( "AddHandleShakeTrigger", "Add HandleShake" ),
		FOnGetContent::CreateSP( this, &FHandleShakeTrackEditor::BuildAddVisibilityTriggerMenu, Track ),
		Params.NodeIsHovered,GetSequencer() );
}


void FHandleShakeTrackEditor::BuildTrackContextMenu(FMenuBuilder& MenuBuilder, UMovieSceneTrack* Track)
{
	UMovieSceneHandleShakeTrack* HandleShakeTrack = Cast<UMovieSceneHandleShakeTrack>(Track);

	class FHandleShakeTrackCustomization : public IDetailCustomization
	{
	public:
		FHandleShakeTrackCustomization()
		{
		}

		~FHandleShakeTrackCustomization()
		{
		}

		virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
		{
			DetailBuilder.HideCategory("Track");
			DetailBuilder.HideCategory("General");
		}

	};

	auto SubMenuDelegate = [HandleShakeTrack](FMenuBuilder& InMenuBuilder, UMovieSceneHandleShakeTrack* InHandleShakeTrack)
	{
		FDetailsViewArgs DetailsViewArgs;
		{
			DetailsViewArgs.bAllowSearch = false;
			DetailsViewArgs.bCustomFilterAreaLocation = true;
			DetailsViewArgs.bCustomNameAreaLocation = true;
			DetailsViewArgs.bHideSelectionTip = true;
			DetailsViewArgs.bLockable = false;
			DetailsViewArgs.bSearchInitialKeyFocus = true;
			DetailsViewArgs.bUpdatesFromSelection = false;
			DetailsViewArgs.bShowOptions = false;
			DetailsViewArgs.bShowModifiedPropertiesOption = false;
			//DetailsViewArgs.NotifyHook = &DetailsNotifyWrapper.Get();
		}

		TArray<TWeakObjectPtr<UObject>> Tracks;
		{
			Tracks.Add(HandleShakeTrack);
		}

		TSharedRef<IDetailsView> DetailsView = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor").CreateDetailView(DetailsViewArgs);

		FOnGetDetailCustomizationInstance CreateInstance = FOnGetDetailCustomizationInstance::CreateLambda([=] { return MakeShared<FHandleShakeTrackCustomization>(); });
		DetailsView->RegisterInstancedCustomPropertyLayout(UMovieSceneHandleShakeTrack::StaticClass(), CreateInstance);
		{
			DetailsView->SetObjects(Tracks);
		}

		InMenuBuilder.AddWidget(DetailsView, FText::GetEmpty(), true);
	};

	const bool bInOpenSubMenuOnClick = false;

	MenuBuilder.AddSubMenu(
		LOCTEXT("Properties", "Properties"),
		LOCTEXT("PropertiesTooltip", ""),
		FNewMenuDelegate::CreateLambda(SubMenuDelegate, HandleShakeTrack),
		bInOpenSubMenuOnClick);

	MenuBuilder.AddSubMenu(
		LOCTEXT("HandleShakeData", "HandleShakeData"),
		LOCTEXT("HandleShakeDataTooltip", ""),
		FNewMenuDelegate::CreateSP(this, &FHandleShakeTrackEditor::AddHandleShakeDataItems, Track));
}

void FHandleShakeTrackEditor::AddNewSection( UMovieScene* MovieScene, UMovieSceneTrack* HandleShakeTrack)
{
	const FScopedTransaction Transaction( LOCTEXT( "AddHandleShakeSection_Transaction", "Add HandleShake Trigger" ) );
	HandleShakeTrack->Modify();

	UMovieSceneHandleShakeSection* HandleShakeSection = CastChecked<UMovieSceneHandleShakeSection>(HandleShakeTrack->CreateNewSection() );

	TRange<FFrameNumber> SectionRange = MovieScene->GetPlaybackRange();
	HandleShakeSection->SetRange(SectionRange);

	int32 RowIndex = -1;
	for ( const UMovieSceneSection* Section : HandleShakeTrack->GetAllSections() )
	{
		RowIndex = FMath::Max( RowIndex, Section->GetRowIndex() );
	}
	HandleShakeSection->SetRowIndex(RowIndex + 1);

	HandleShakeTrack->AddSection( *HandleShakeSection);

}


void FHandleShakeTrackEditor::OnAddTrack()
{
	UMovieScene* FocusedMovieScene = GetFocusedMovieScene();

	if ( FocusedMovieScene == nullptr )
	{
		return;
	}

	const FScopedTransaction Transaction( LOCTEXT( "AddHandleShakeTrack_Transaction", "Add HandleShake Track" ) );
	FocusedMovieScene->Modify();

	UMovieSceneHandleShakeTrack* NewTrack = FocusedMovieScene->AddMasterTrack<UMovieSceneHandleShakeTrack>();
	checkf( NewTrack != nullptr, TEXT("Failed to create new HandleShake track.") );

	//AddNewSection( FocusedMovieScene, NewTrack);

	GetSequencer()->NotifyMovieSceneDataChanged( EMovieSceneDataChangeType::MovieSceneStructureItemAdded );
}


TSharedRef<SWidget> FHandleShakeTrackEditor::BuildAddVisibilityTriggerMenu( UMovieSceneTrack* HandleShakeTrack )
{
	FMenuBuilder MenuBuilder(true, nullptr);

	MenuBuilder.AddMenuEntry(
		LOCTEXT( "AddHandleShakeTrigger", "HandleShake" ),
		LOCTEXT( "AddHandleShakeTriggerToolTip", "Add a HandleShake." ),
		FSlateIcon(),
		FUIAction( FExecuteAction::CreateSP(
			this, &FHandleShakeTrackEditor::OnAddNewSection, HandleShakeTrack) ) );
	return MenuBuilder.MakeWidget();
}


void FHandleShakeTrackEditor::OnAddNewSection( UMovieSceneTrack* HandleShakeTrack)
{
	UMovieScene* FocusedMovieScene = GetFocusedMovieScene();

	if ( FocusedMovieScene == nullptr )
	{
		return;
	}

	AddNewSection( FocusedMovieScene, HandleShakeTrack);
	GetSequencer()->NotifyMovieSceneDataChanged( EMovieSceneDataChangeType::MovieSceneStructureItemAdded );
}

void FHandleShakeTrackEditor::OnGenerateSkipKeys(UMovieSceneTrack* tempHandleShakeTrack)
{

}

#undef LOCTEXT_NAMESPACE