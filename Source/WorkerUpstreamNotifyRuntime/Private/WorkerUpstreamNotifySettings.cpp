// Copyright ubyte digital. All Right Reserved. https://ubyte.dev

#include "WorkerUpstreamNotifySettings.h"


void UWorkerUpstreamNotifySettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeProperty(PropertyChangedEvent);
}

void UWorkerUpstreamNotifySettings::PostReloadConfig(FProperty* PropertyThatWasLoaded)
{
	UObject::PostReloadConfig(PropertyThatWasLoaded);
}

void UWorkerUpstreamNotifySettings::PostInitProperties()
{
	UObject::PostInitProperties();
	UpdateGameplayConceptConfig();
}

TArray<FString> UWorkerUpstreamNotifySettings::GetAllRelevantBaseCategories() const
{
	return WorkerUpstreamNotifyBaseCategories;
}

void UWorkerUpstreamNotifySettings::UpdateRelevantCategoryRemapsDisplay(const TArray<FGameplayTagCategoryRemap>& Remaps)
{
	RelevantCategoryRemaps = Remaps;
}

