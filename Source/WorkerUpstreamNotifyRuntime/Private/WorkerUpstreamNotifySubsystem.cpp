// Copyright ubyte digital. All Right Reserved. https://ubyte.dev

#include "WorkerUpstreamNotifySubsystem.h"
#include "WorkerUpstreamNotifySettings.h"
#include "GameplayTagsSettings.h"

IMPLEMENT_MODULE(FDefaultModuleImpl, WorkerUpstreamNotifyRuntime)

DEFINE_LOG_CATEGORY_STATIC(LogWorkerUpstreamNotify, Log, All)

void UWorkerUpstreamNotifySubsystem::NotifyCrowdActivityWorker(const IGenericAgentActivityWorker* AgentActivityWorker, FAgentNotifyTag AgentNotifyTag)
{
	if (ensure(AgentActivityWorker != nullptr) && AgentActivityWorker->IsAgentActivityContextValid() && AgentNotifyTag.IsValid())
	{
		UE_LOG(LogWorkerUpstreamNotify, Verbose, TEXT("%hs (%s, %s)"), __func__, *GetNameSafe(Cast<UObject>(AgentActivityWorker)), *AgentNotifyTag.ToString());
		
		// Forward the context
		OnAgentNotify().Broadcast(AgentActivityWorker->GetAgentActivityContext(), AgentNotifyTag);
	}
}

void UWorkerUpstreamNotifySubsystem::NotifyWorldActivityWorker(const IGenericCrowdActivityWorker* CrowdActivityWorker, FCrowdNotifyTag CrowdNotifyTag)
{
	if (ensure(CrowdActivityWorker != nullptr) && CrowdActivityWorker->IsCrowdActivityContextValid() && CrowdNotifyTag.IsValid())
	{
		UE_LOG(LogWorkerUpstreamNotify, Verbose, TEXT("%hs (%s, %s)"), __func__, *GetNameSafe(Cast<UObject>(CrowdActivityWorker)), *CrowdNotifyTag.ToString());
		
		// Forward the context
		OnCrowdNotify().Broadcast(CrowdActivityWorker->GetCrowdActivityContext(), CrowdNotifyTag);
	}
}

UAsyncAction_WaitAnyAgentNotify* UAsyncAction_WaitAnyAgentNotify::WaitAnyAgentNotify(UObject* WorldContextObject,
	TScriptInterface<IGenericCrowdActivityWorker> CrowdActivityWorker, FAgentNotifyTag AgentNotifyTag, bool bOnlyTriggerOnce)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return nullptr;
	}
	
	UAsyncAction_WaitAnyAgentNotify* Action = NewObject<UAsyncAction_WaitAnyAgentNotify>();
	Action->RegisterWithGameInstance(WorldContextObject);
	Action->WeakCrowdActivityWorker = CrowdActivityWorker;
	Action->ListeningAgentNotifyTag = AgentNotifyTag;
	Action->OnlyTriggerOnce = bOnlyTriggerOnce;
	return Action;
}

void UAsyncAction_WaitAnyAgentNotify::Activate()
{
	Super::Activate();

	if (!OnAgentNotifyCallbackDelegateHandle.IsValid())
	{
		OnAgentNotifyCallbackDelegateHandle =
			GEngine->GetEngineSubsystem<UWorkerUpstreamNotifySubsystem>()->OnAgentNotify().AddUObject(this, &ThisClass::OnAgentNotifyCallback);
	}
}

void UAsyncAction_WaitAnyAgentNotify::EndAction()
{
	if (OnAgentNotifyCallbackDelegateHandle.IsValid())
	{
		GEngine->GetEngineSubsystem<UWorkerUpstreamNotifySubsystem>()->OnAgentNotify().Remove(OnAgentNotifyCallbackDelegateHandle);
		OnAgentNotifyCallbackDelegateHandle.Reset();
	}

	SetReadyToDestroy();
}

void UAsyncAction_WaitAnyAgentNotify::OnAgentNotifyCallback(const FAgentActivityContext& AgentActivityContext, const FAgentNotifyTag& AgentNotifyTag)
{
	if (AgentNotifyTag == ListeningAgentNotifyTag)
	{
		EventReceived.Broadcast(AgentActivityContext.GetWorker<UObject>());
	
		if (OnlyTriggerOnce)
		{
			EndAction();
		}
	}
}

UAsyncAction_WaitAnyAgentNotifyFiltered* UAsyncAction_WaitAnyAgentNotifyFiltered::WaitAnyAgentNotifyFiltered(
	UObject* WorldContextObject, TScriptInterface<IGenericCrowdActivityWorker> CrowdActivityWorker,
	FAgentNotifyTag AgentNotifyTag, FGameplayTagQuery AgentActivityCognitionFilter, bool bOnlyTriggerOnce)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return nullptr;
	}
	
	UAsyncAction_WaitAnyAgentNotifyFiltered* Action = NewObject<UAsyncAction_WaitAnyAgentNotifyFiltered>();
	Action->RegisterWithGameInstance(WorldContextObject);
	Action->WeakCrowdActivityWorker = CrowdActivityWorker;
	Action->ListeningAgentNotifyTag = AgentNotifyTag;
	Action->ActivityWorkerCognitionFilter = AgentActivityCognitionFilter;
	Action->OnlyTriggerOnce = bOnlyTriggerOnce;
	return Action;
}

void UAsyncAction_WaitAnyAgentNotifyFiltered::Activate()
{
	Super::Activate();

	if (!OnAgentNotifyCallbackDelegateHandle.IsValid())
	{
		OnAgentNotifyCallbackDelegateHandle =
			GEngine->GetEngineSubsystem<UWorkerUpstreamNotifySubsystem>()->OnAgentNotify().AddUObject(this, &ThisClass::OnAgentNotifyCallback);
	}
}

void UAsyncAction_WaitAnyAgentNotifyFiltered::EndAction()
{
	if (OnAgentNotifyCallbackDelegateHandle.IsValid())
	{
		GEngine->GetEngineSubsystem<UWorkerUpstreamNotifySubsystem>()->OnAgentNotify().Remove(OnAgentNotifyCallbackDelegateHandle);
		OnAgentNotifyCallbackDelegateHandle.Reset();
	}

	SetReadyToDestroy();
}

void UAsyncAction_WaitAnyAgentNotifyFiltered::OnAgentNotifyCallback(const FAgentActivityContext& AgentActivityContext,
	const FAgentNotifyTag& InAgentNotifyTag)
{
	if (AgentActivityContext.GetWorker()->MatchesAgentActivityCognition(ActivityWorkerCognitionFilter))
	{
		EventReceived.Broadcast(AgentActivityContext.GetWorker<UObject>());

		if (OnlyTriggerOnce)
		{
			EndAction();
		}
	}
}

UAsyncAction_WaitAnyCrowdNotify* UAsyncAction_WaitAnyCrowdNotify::WaitAnyCrowdNotify(UObject* WorldContextObject,
                                                                                     TScriptInterface<IGenericWorldActivityWorker> WorldActivityWorker, FCrowdNotifyTag CrowdNotifyTag,
                                                                                     bool bOnlyTriggerOnce)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return nullptr;
	}

	if (!GEngine->GetEngineSubsystem<UWorkerUpstreamNotifySubsystem>())
	{
		UE_LOG(LogWorkerUpstreamNotify, Error, TEXT("%s failed. Engine subsystem '%s' is required!"), PREPROCESSOR_TO_STRING(UWorkerUpstreamNotifyGameplayConcept));
		return nullptr;
	}
	
	UAsyncAction_WaitAnyCrowdNotify* Action = NewObject<UAsyncAction_WaitAnyCrowdNotify>();
	Action->RegisterWithGameInstance(WorldContextObject);
	Action->WeakWorldActivityWorker = WorldActivityWorker;
	Action->ListeningCrowdNotifyTag = CrowdNotifyTag;
	Action->OnlyTriggerOnce = bOnlyTriggerOnce;
	return Action;
}

void UAsyncAction_WaitAnyCrowdNotify::Activate()
{
	Super::Activate();
	
	if (!OnCrowdNotifyCallbackDelegateHandle.IsValid())
	{
		OnCrowdNotifyCallbackDelegateHandle =
			GEngine->GetEngineSubsystem<UWorkerUpstreamNotifySubsystem>()->OnCrowdNotify().AddUObject(this, &ThisClass::OnCrowdNotifyCallback);
	}
}

void UAsyncAction_WaitAnyCrowdNotify::EndAction()
{
	if (OnCrowdNotifyCallbackDelegateHandle.IsValid())
	{
		GEngine->GetEngineSubsystem<UWorkerUpstreamNotifySubsystem>()->OnCrowdNotify().Remove(OnCrowdNotifyCallbackDelegateHandle);
		OnCrowdNotifyCallbackDelegateHandle.Reset();
	}

	SetReadyToDestroy();
}

void UAsyncAction_WaitAnyCrowdNotify::OnCrowdNotifyCallback(const FCrowdActivityContext& CrowdActivityContext,
	const FCrowdNotifyTag& InCrowdNotifyTag)
{
	if (InCrowdNotifyTag == ListeningCrowdNotifyTag)
	{
		EventReceived.Broadcast(CrowdActivityContext.GetWorker<UObject>());

		if (OnlyTriggerOnce)
		{
			EndAction();
		}
	}
}

UAsyncAction_WaitAnyCrowdNotifyFiltered* UAsyncAction_WaitAnyCrowdNotifyFiltered::WaitAnyCrowdNotifyFiltered(
	UObject* WorldContextObject, TScriptInterface<IGenericWorldActivityWorker> WorldActivityWorker,
	FCrowdNotifyTag CrowdNotifyTag, FGameplayTagQuery CrowdActivityCognitionFilter, bool bOnlyTriggerOnce)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return nullptr;
	}
	
	UAsyncAction_WaitAnyCrowdNotifyFiltered* Action = NewObject<UAsyncAction_WaitAnyCrowdNotifyFiltered>();
	Action->RegisterWithGameInstance(WorldContextObject);
	Action->WeakWorldActivityWorker = WorldActivityWorker;
	Action->ListeningCrowdNotifyTag = CrowdNotifyTag;
	Action->CrowdActivityCognitionFilter = CrowdActivityCognitionFilter;
	Action->OnlyTriggerOnce = bOnlyTriggerOnce;
	return Action;
}

void UAsyncAction_WaitAnyCrowdNotifyFiltered::Activate()
{
	Super::Activate();
	
	if (!OnCrowdNotifyCallbackDelegateHandle.IsValid())
	{
		OnCrowdNotifyCallbackDelegateHandle =
			GEngine->GetEngineSubsystem<UWorkerUpstreamNotifySubsystem>()->OnCrowdNotify().AddUObject(this, &ThisClass::OnCrowdNotifyCallback);
	}
}

void UAsyncAction_WaitAnyCrowdNotifyFiltered::EndAction()
{
	if (OnCrowdNotifyCallbackDelegateHandle.IsValid())
	{
		GEngine->GetEngineSubsystem<UWorkerUpstreamNotifySubsystem>()->OnCrowdNotify().Remove(OnCrowdNotifyCallbackDelegateHandle);
		OnCrowdNotifyCallbackDelegateHandle.Reset();
	}

	SetReadyToDestroy();
}

void UAsyncAction_WaitAnyCrowdNotifyFiltered::OnCrowdNotifyCallback(const FCrowdActivityContext& CrowdActivityContext,
	const FCrowdNotifyTag& InCrowdNotifyTag)
{
	if (CrowdActivityContext.GetWorker()->MatchesCrowdActivityCognition(CrowdActivityCognitionFilter))
	{
		EventReceived.Broadcast(CrowdActivityContext.GetWorker<UObject>());

		if (OnlyTriggerOnce)
		{
			EndAction();
		}
	}
}



