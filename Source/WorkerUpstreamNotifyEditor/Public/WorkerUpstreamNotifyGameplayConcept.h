// Copyright ubyte digital. All Right Reserved. https://ubyte.dev

#pragma once

#include "CoreMinimal.h"
#include "INativeGameplayConcept.h"

/**
 * Extending GenericActivityFramework,
 * WorkerUpstreamNotify allows workers to send notifications to their higher initiator, which may or may not listen to them.
 * - The emphasis lies in the explicitness of notifying a higher worker about something that shouldn't necessarily be known by everyone publicly.
 *
 * Important: This does *not* replace the Gameplay Ability System whatsoever. 
 * - You can use the Gameplay Ability System to handle everything internally, while letting this EngineSubsystem merely acts as public rear-end to other systems.
 *
 * 
 *                 WorkerUpstreamNotify         (Native-only. Introduces by this module)
 *                           |
 *               ------------------------
 *               |                      |
 *    WorkerGameplayUpshot             ...      (E.g. Blueprint-exposed derivative. Introduces by another module)
 *
 */
class WORKERUPSTREAMNOTIFYEDITOR_API IWorkerUpstreamNotifyGameplayConcept : public INativeGameplayConcept
{
public:
	virtual TArray<FString> GetWorldNotifyBaseRemapCategories() const { return TArray<FString>(); }
	virtual TArray<FString> GetCrowdNotifyBaseRemapCategories() const { return TArray<FString>(); }
	virtual TArray<FString> GetAgentNotifyBaseRemapCategories() const { return TArray<FString>(); }

	// Helper function to generate all category remaps this gameplay concept stands for
	virtual TArray<FGameplayTagCategoryRemap> GenerateDesiredCategoryRemaps() override;
	virtual TObjectPtr<INativeGameplayConceptConfig> GetGameplayConceptConfig() override;
};
