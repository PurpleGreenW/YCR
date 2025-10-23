// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class YCR : ModuleRules
{
    public YCR(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        // Performance Optimierungen
        bLegacyPublicIncludePaths = false;
        MinFilesUsingPrecompiledHeaderOverride = 1;
        bUseUnity = false;
        
        // C++ Standard
        CppStandard = CppStandardVersion.Cpp20;
        
        // Warnungen als Fehler
        bEnableExceptions = true;
        
        // =====================================================
        // Core Dependencies
        // =====================================================
        PublicDependencyModuleNames.AddRange(new string[] 
        { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore",
            "HeadMountedDisplay",
            "NavigationSystem",
            "AIModule",
            "GameplayTasks",
            "UMG",
            "Slate",
            "SlateCore"
        });
        
        // =====================================================
        // Steam Integration (für Leaderboards)
        // =====================================================
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "OnlineSubsystem",
            "OnlineSubsystemSteam",
            "OnlineSubsystemUtils"
        });
        
        // =====================================================
        // Landscape & Water
        // =====================================================
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Landscape",
            "Water"
        });
        
        // Falls Landmass benötigt wird (optional):
        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(new string[]
            {
                "UnrealEd",
                "LandscapeEditor"
            });
        }
        
        // =====================================================
        // Gameplay Dependencies
        // =====================================================
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "EnhancedInput",
            "MotionWarping",
            "AnimGraphRuntime"
        });
        
        // =====================================================
        // Rendering & Effects
        // =====================================================
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Niagara",
            "NiagaraCore",
            "RenderCore",
            "RHI"
        });
        
        // =====================================================
        // Private Dependencies
        // =====================================================
        PrivateDependencyModuleNames.AddRange(new string[] 
        { 
            "Json",
            "JsonUtilities",
            "HTTP",
            "DeveloperSettings"
        });
        
        // =====================================================
        // Include Paths
        // =====================================================
        PublicIncludePaths.AddRange(new string[]
        {
            "YCR/Public/Core",
            "YCR/Public/Character",
            "YCR/Public/Enemies", 
            "YCR/Public/Utils",
            "YCR/Public/Utils/Enums",
            "YCR/Public/Utils/Interfaces",
            "YCR/Public/Data",
        });
        
        // =====================================================
        // Platform-spezifisch
        // =====================================================
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicDefinitions.Add("YCR_PLATFORM_WINDOWS=1");
        }
    }
}