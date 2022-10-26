// Copyright ubyte digital. All Right Reserved. https://ubyte.dev

#pragma once

#include "CoreMinimal.h"
#include "IGenericAgentActivityWorker.h"
#include "IGenericWorldActivityWorker.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WorkerUpstreamNotifySubsystem.generated.h"

// WorkerUpstreamNotifyRuntime (Module) claims FAgentNotifyTag, FCrowdNotifyTag and FWorldNotifyTag struct names in global scope.
// Reason: this gameplay concept should never be used in conjunction with other 'Notify' style gameplay concepts that extend GenericActivityFramework.

// Map your tags or subcategories to "AgentNotifyBase" this instead of using "AgentNotifyBase" as GameplayTag root directly!
USTRUCT(Meta = (Categories = "AgentNotifyBase"))
struct FAgentNotifyTag : public FGameplayTag
{
	GENERATED_BODY()
	END_TYPED_TAG_DECL(FAgentNotifyTag, TEXT(""))
};

// Map your tags or subcategories to "CrowdNotifyBase" this instead of using "CrowdNotifyBase" as GameplayTag root directly!
USTRUCT(Meta = (Categories = "CrowdNotifyBase"))
struct FCrowdNotifyTag : public FGameplayTag
{
	GENERATED_BODY()
	END_TYPED_TAG_DECL(FCrowdNotifyTag, TEXT(""))
};

// Map your tags or subcategories to "WorldNotifyBase" this instead of using "WorldNotifyBase" as GameplayTag root directly!
USTRUCT(Meta = (Categories = "WorldNotifyBase"))
struct FWorldNotifyTag : public FGameplayTag
{
	GENERATED_BODY()
	END_TYPED_TAG_DECL(FWorldNotifyTag, TEXT(""))
};

// @Future
USTRUCT() struct FWorldNotifyFieldSetBase { GENERATED_BODY() };
USTRUCT() struct FCrowdNotifyFieldSetBase { GENERATED_BODY() };
USTRUCT() struct FAgentNotifyFieldSetBase { GENERATED_BODY() };
//~future

UCLASS()
class WORKERUPSTREAMNOTIFYRUNTIME_API UWorkerUpstreamNotifySubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	using FAgentNotifyDelegate = TMulticastDelegate<void(const FAgentActivityContext&, const FAgentNotifyTag&)>;
	using FCrowdNotifyDelegate = TMulticastDelegate<void(const FCrowdActivityContext&, const FCrowdNotifyTag&)>;

	// Get this engine subsystem, assuming GEngine is valid
	static UWorkerUpstreamNotifySubsystem* Get()
	{
		check(GEngine);
		return GEngine->GetEngineSubsystem<UWorkerUpstreamNotifySubsystem>();
	}
	
	/** Notify an agent's CrowdActivityWorker about a generic event */
	void NotifyCrowdActivityWorker(const IGenericAgentActivityWorker* AgentActivityWorker, FAgentNotifyTag AgentNotifyTag);

	/** Notify a crowd's WorldActivityWorker about a generic event */
	void NotifyWorldActivityWorker(const IGenericCrowdActivityWorker* CrowdActivityWorker, FCrowdNotifyTag CrowdNotifyTag);

	/** Broadcasts when an AgentActivityWorker notifies its CrowdActivityWorker about a generic event */
	FORCEINLINE FAgentNotifyDelegate& OnAgentNotify() { return OnAgentNotifyDelegate; }

	/** Broadcasts when a CrowdActivityWorker notifies its WorldActivityWorker about a generic event */
	FORCEINLINE FCrowdNotifyDelegate& OnCrowdNotify() { return OnCrowdNotifyDelegate; }
	
private:
	FAgentNotifyDelegate OnAgentNotifyDelegate;
	FCrowdNotifyDelegate OnCrowdNotifyDelegate;
};

UCLASS()
class WORKERUPSTREAMNOTIFYRUNTIME_API UAsyncAction_WaitAnyAgentNotify : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventReceivedDelegate, TScriptInterface<IGenericAgentActivityWorker>, AgentActivityWorker);
	
	/** Wait for an agent-specific notification from any AgentActivityWorker this CrowdActivityWorker is associated with */
	UFUNCTION(BlueprintCallable, Meta = (WorldContext = WorldContextObject, BlueprintInternalUseOnly = True, DisplayName = "Wait Any Agent Notify (Crowd Activity Worker)"))
	static UAsyncAction_WaitAnyAgentNotify* WaitAnyAgentNotify(UObject* WorldContextObject, UPARAM(Meta = (DefaultToSelf = "CrowdActivityWorker")) TScriptInterface<IGenericCrowdActivityWorker> CrowdActivityWorker, FAgentNotifyTag AgentNotifyTag, bool bOnlyTriggerOnce);
	
	UPROPERTY(BlueprintAssignable)
	FEventReceivedDelegate EventReceived;
	
protected:
	virtual void Activate() override;
	virtual void EndAction();
	
	virtual void OnAgentNotifyCallback(const FAgentActivityContext& AgentActivityContext, const FAgentNotifyTag& InAgentNotifyTag);
	
	FDelegateHandle OnAgentNotifyCallbackDelegateHandle;
	TWeakInterfacePtr<IGenericCrowdActivityWorker> WeakCrowdActivityWorker;
	FAgentNotifyTag ListeningAgentNotifyTag;
	
	bool OnlyTriggerOnce = false;
};

UCLASS()
class WORKERUPSTREAMNOTIFYRUNTIME_API UAsyncAction_WaitAnyAgentNotifyFiltered : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventReceivedDelegate, TScriptInterface<IGenericAgentActivityWorker>, AgentActivityWorker);

	/** Wait for an agent-specific notification from any AgentActivityWorker this CrowdActivityWorker is associated with (filtered) */
	UFUNCTION(BlueprintCallable, Meta = (WorldContext = WorldContextObject, BlueprintInternalUseOnly = True, DisplayName = "Wait Any Agent Notify Filtered (Crowd Activity Worker)"))
	static UAsyncAction_WaitAnyAgentNotifyFiltered* WaitAnyAgentNotifyFiltered(UObject* WorldContextObject, UPARAM(Meta = (DefaultToSelf = "CrowdActivityWorker")) TScriptInterface<IGenericCrowdActivityWorker> CrowdActivityWorker, FAgentNotifyTag AgentNotifyTag, UPARAM(Meta = (Category = "AgentActivityCognition")) FGameplayTagQuery AgentActivityCognitionFilterTags, bool bOnlyTriggerOnce);
	
	UPROPERTY(BlueprintAssignable)
	FEventReceivedDelegate EventReceived;

protected:
	virtual void Activate() override;
	virtual void EndAction();
	
	virtual void OnAgentNotifyCallback(const FAgentActivityContext& AgentActivityContext, const FAgentNotifyTag& InAgentNotifyTag);
	
	FDelegateHandle OnAgentNotifyCallbackDelegateHandle;
	TWeakInterfacePtr<IGenericCrowdActivityWorker> WeakCrowdActivityWorker;
	FAgentNotifyTag ListeningAgentNotifyTag;
	FGameplayTagQuery ActivityWorkerCognitionFilter;
	
	bool OnlyTriggerOnce = false;
};

UCLASS()
class WORKERUPSTREAMNOTIFYRUNTIME_API UAsyncAction_WaitAnyCrowdNotify : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventReceivedDelegate, TScriptInterface<IGenericCrowdActivityWorker>, CrowdActivityWorker);

	/** Wait for a crowd-specific notification from any CrowdActivityWorker this WorldActivityWorker is associated with */
	UFUNCTION(BlueprintCallable, Meta = (WorldContext = WorldContextObject, BlueprintInternalUseOnly = True, DisplayName = "Wait Any Crowd Notify (World Activity Worker)"))
	static UAsyncAction_WaitAnyCrowdNotify* WaitAnyCrowdNotify(UObject* WorldContextObject, UPARAM(Meta = (DefaultToSelf = "WorldActivityWorker")) TScriptInterface<IGenericWorldActivityWorker> WorldActivityWorker, FCrowdNotifyTag CrowdNotifyTag, bool bOnlyTriggerOnce);
	
	UPROPERTY(BlueprintAssignable)
	FEventReceivedDelegate EventReceived;

protected:
	virtual void Activate() override;
	virtual void EndAction();
	
	virtual void OnCrowdNotifyCallback(const FCrowdActivityContext& CrowdActivityContext, const FCrowdNotifyTag& InCrowdNotifyTag);
	
	FDelegateHandle OnCrowdNotifyCallbackDelegateHandle;
	TWeakInterfacePtr<IGenericWorldActivityWorker> WeakWorldActivityWorker;
	FCrowdNotifyTag ListeningCrowdNotifyTag;
	
	bool OnlyTriggerOnce = false;
};

UCLASS()
class WORKERUPSTREAMNOTIFYRUNTIME_API UAsyncAction_WaitAnyCrowdNotifyFiltered : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventReceivedDelegate, TScriptInterface<IGenericCrowdActivityWorker>, CrowdActivityWorker);
	
	/** Wait for a crowd-specific notification from any CrowdActivityWorker this WorldActivityWorker is associated with (filtered) */
	UFUNCTION(BlueprintCallable, Meta = (WorldContext = WorldContextObject, BlueprintInternalUseOnly = True, DisplayName = "Wait Any Crowd Notify (World Activity Worker)"))
	static UAsyncAction_WaitAnyCrowdNotifyFiltered* WaitAnyCrowdNotifyFiltered(UObject* WorldContextObject, UPARAM(Meta = (DefaultToSelf = "WorldActivityWorker")) TScriptInterface<IGenericWorldActivityWorker> WorldActivityWorker, FCrowdNotifyTag CrowdNotifyTag, UPARAM(Meta = (Category = "CrowdActivityCognition")) FGameplayTagQuery CrowdActivityCognitionFilter, bool bOnlyTriggerOnce);
	
	UPROPERTY(BlueprintAssignable)
	FEventReceivedDelegate EventReceived;

protected:
	virtual void Activate() override;
	virtual void EndAction();
	
	virtual void OnCrowdNotifyCallback(const FCrowdActivityContext& CrowdActivityContext, const FCrowdNotifyTag& InCrowdNotifyTag);
	
	FDelegateHandle OnCrowdNotifyCallbackDelegateHandle;
	TWeakInterfacePtr<IGenericWorldActivityWorker> WeakWorldActivityWorker;
	FCrowdNotifyTag ListeningCrowdNotifyTag;
	FGameplayTagQuery CrowdActivityCognitionFilter;
	
	bool OnlyTriggerOnce = false;
};



