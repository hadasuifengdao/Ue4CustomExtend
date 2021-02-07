// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PaladinHandleShakeTrackEditor : ModuleRules
{
	public PaladinHandleShakeTrackEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        // IWXU
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] {
                "PaladinHandleShakeTrackEditor/Public",
                "PaladinSeven/Public/CustomTracks"
                 });

        PrivateIncludePaths.AddRange(new string[] {
                 "PaladinHandleShakeTrackEditor/Private",
                 "PaladinSeven/Private/CustomTracks"
                  });

     // PrivateIncludePaths.Add("PaladinHandleShakeSkipTrackEditor");

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "UnrealEd",
                "Sequencer",
                "CriWareRuntime",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "ActorPickerMode",
                "DesktopPlatform",
                "LevelSequence",
                "MovieScene",
                "MovieSceneCapture",
                "MovieSceneTools",
                "MovieSceneTracks",
                "BlueprintGraph",
                "KismetCompiler",
                "ContentBrowser",
                "Slate",
                "SlateCore",
                "EditorStyle",
                "EditorWidgets",
                "PropertyEditor",
                "RenderCore",
                "RHI",
                //"ShaderCore",
                "SequenceRecorder",
                "Pal7"
                
            }
        );

        PrivateIncludePathModuleNames.AddRange(
            new string[] {
                "AssetRegistry",
                "AssetTools",
                "Sequencer",
                "Settings",
                "SceneOutliner",
                "MainFrame",
                "UnrealEd",
            }
        );
    }
}
