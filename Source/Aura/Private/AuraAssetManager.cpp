// Copyright AM Company


#include "AuraAssetManager.h"

#include "AuraGameplayTags.h"
// #include "AbilitySystemGlobals.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);

	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();

	// This is required to use Target Data! in 5.3 it's being called within GameplayAbilitiesModule.cpp.
	// UAbilitySystemGlobals::Get().InitGlobalData();
}