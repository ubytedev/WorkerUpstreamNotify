// Copyright ubyte digital. All Right Reserved. https://ubyte.dev

#pragma once

#include "CoreMinimal.h"
#include "WorkerUpstreamNotifyGameplayConcept.h"
#include "Modules/ModuleInterface.h"

class FWorkerUpstreamNotifyEditorModule : public IModuleInterface, public IWorkerUpstreamNotifyGameplayConcept
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};


