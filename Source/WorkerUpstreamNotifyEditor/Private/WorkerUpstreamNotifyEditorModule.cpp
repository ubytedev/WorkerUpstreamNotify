// Copyright ubyte digital. All Right Reserved. https://ubyte.dev


#include "WorkerUpstreamNotifyEditorModule.h"
#include "WorkerUpstreamNotifySettings.h"
#include "GameplayTagsEditorModule.h"
#include "ISettingsModule.h"
#include "WorkerUpstreamNotifySubsystem.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FWorkerUpstreamNotifyEditorModule"

IMPLEMENT_MODULE(FWorkerUpstreamNotifyEditorModule, WorkerUpstreamNotifyEditor)

void FWorkerUpstreamNotifyEditorModule::StartupModule()
{
	ApplyGameplayConcept();

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		const FString PluginDesc = IPluginManager::Get().FindPlugin(UE_PLUGIN_NAME)->GetDescriptor().Description;
		
		SettingsModule->RegisterSettings("Project", "Plugins", "Worker Upstream Notify",
			LOCTEXT("WorkerUpstreamNotifySettingsName", "Worker Upstream Notify"),
			FText::Format(LOCTEXT("WorkerUpstreamNotifySettingsNameDesc", "{0}"), FText::FromString(PluginDesc)),
			GetMutableDefault<UWorkerUpstreamNotifySettings>()
		);
	}

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(FWorldNotifyTag::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGameplayTagCustomizationPublic::MakeInstance));
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(FCrowdNotifyTag::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGameplayTagCustomizationPublic::MakeInstance));
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(FAgentNotifyTag::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGameplayTagCustomizationPublic::MakeInstance));
}

void FWorkerUpstreamNotifyEditorModule::ShutdownModule()
{
	// Unregister settings are in Project Settings
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "Worker State Event");
	}

	if (FModuleManager::Get().IsModuleLoaded(TEXT("PropertyEditor")))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
		PropertyModule.UnregisterCustomPropertyTypeLayout(FWorldNotifyTag::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomPropertyTypeLayout(FCrowdNotifyTag::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomPropertyTypeLayout(FAgentNotifyTag::StaticStruct()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE
