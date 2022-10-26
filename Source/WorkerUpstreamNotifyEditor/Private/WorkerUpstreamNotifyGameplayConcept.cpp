// Copyright ubyte digital. All Right Reserved. https://ubyte.dev


#include "WorkerUpstreamNotifyGameplayConcept.h"

TArray<FGameplayTagCategoryRemap> IWorkerUpstreamNotifyGameplayConcept::GenerateDesiredCategoryRemaps()
{
	TArray<FGameplayTagCategoryRemap> Result;
		
	auto AddRemap = [&Result](FString BaseCategory, TArray<FString> RemapCategories)
	{
		if (RemapCategories.IsEmpty())
		{
			return;
		}
		
		FGameplayTagCategoryRemap Remap;
		Remap.BaseCategory = MoveTemp(BaseCategory);
		Remap.RemapCategories = MoveTemp(RemapCategories);
		Result.Add(MoveTemp(Remap));
	};

	AddRemap("AgentNotifyBase", GetAgentNotifyBaseRemapCategories());
	AddRemap("CrowdNotifyBase", GetCrowdNotifyBaseRemapCategories());
	AddRemap("WorldNotifyBase", GetWorldNotifyBaseRemapCategories());
		
	return Result;
}

TObjectPtr<INativeGameplayConceptConfig> IWorkerUpstreamNotifyGameplayConcept::GetGameplayConceptConfig()
{
	return GetMutableDefault<UWorkerUpstreamNotifySettings>();
}
