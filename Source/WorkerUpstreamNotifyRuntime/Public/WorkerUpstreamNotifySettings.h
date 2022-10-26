// Copyright ubyte digital. All Right Reserved. https://ubyte.dev

#pragma once

#include "CoreMinimal.h"
#include "INativeGameplayConceptConfig.h"
#include "GameplayTagsSettings.h"
#include "WorkerUpstreamNotifySettings.generated.h"

UCLASS(Config = WorkerUpstreamNotify, DefaultConfig, NotPlaceable)
class WORKERUPSTREAMNOTIFYRUNTIME_API UWorkerUpstreamNotifySettings : public UObject, public INativeGameplayConceptConfig
{
	GENERATED_BODY()
	
public:
	//~UObject
	#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual void PostReloadConfig(FProperty* PropertyThatWasLoaded) override;
	virtual void PostInitProperties() override;
	//~End UObject

	//~ Begin INativeGameplayConceptConfig
	virtual TArray<FString> GetAllRelevantBaseCategories() const override;
	virtual void UpdateRelevantCategoryRemapsDisplay(const TArray<FGameplayTagCategoryRemap>& Remaps) override;
	//~ End INativeGameplayConceptConfig
	
protected:
	/**
	 * Base for remap categories this plugin statically utilizes (C++). These shouldn't be GameplayTags!
	 * These serve as Category/GameplayTagFilter metadata specifiers throughout the codebase.
	 */
	UPROPERTY(VisibleDefaultsOnly, Meta = (Category = BaseCategory))
	TArray<FString> WorkerUpstreamNotifyBaseCategories =
	{
		"AgentNotifyBase",
		"CrowdNotifyBase",
		"WorldNotifyBase"
	};

	UPROPERTY(VisibleDefaultsOnly, Meta = (Category = Extra))
	TArray<FGameplayTagCategoryRemap> RelevantCategoryRemaps;
};

