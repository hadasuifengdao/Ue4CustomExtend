#include "PaladinHandleShakeTrackEditor.h"
#include "Modules/ModuleManager.h"
#include "ISequencerModule.h"
#include "HandleShakeTrackEditor.h"

DEFINE_LOG_CATEGORY(LogHandleShake);

void PaladinHandleShakeTrackEditorModule::StartupModule()
{
	ISequencerModule& SequencerModule = FModuleManager::Get().LoadModuleChecked<ISequencerModule>("Sequencer");
	HandleShakeTrackEditorHandle = SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FHandleShakeTrackEditor::CreateTrackEditor));


	//UE_LOG(LogTemp, Warning, TEXT("PaladinHandleShakeTrackEditorModule Start"));
}

void PaladinHandleShakeTrackEditorModule::ShutdownModule()
{
	if (!FModuleManager::Get().IsModuleLoaded("Sequencer"))
	{
		return;
	}

	ISequencerModule& SequencerModule = FModuleManager::Get().GetModuleChecked<ISequencerModule>("Sequencer");

	// unregister property track editors
	SequencerModule.UnRegisterTrackEditor(HandleShakeTrackEditorHandle);
}

IMPLEMENT_MODULE(PaladinHandleShakeTrackEditorModule, PaladinHandleShakeTrackEditor)
