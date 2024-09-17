// Copyright AM Company


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{

	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		/*
		* Когда данные от клиента доходят до сервера - происходит рассылка через делегат
		* И можно получить этот делегат через AbilitySystemComponent
		*/

		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle(); 
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey(); // Ключ который был сгенерирован при вызове TryActivateAbility (активации способности с клиента)

		// Как только на сервере будет вызвана активация, сервер свяжет делегат с функцией
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		// Проверка дошел ли делегат до сервера
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			// Заставить сервер ожидать данных от игрока
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	// Создание окна прогнозирования с ограниченной областью 
	// (Окно, в течении которого, все что делаем локально - будет предсказано)
	// Область ограничена текущей функцией
	// GameplayPrediction.h
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	/** Target data with a single hit result, data is packed into the hit result */
	// GameplayAbilityTargetTypes.h
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;

	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);

	/*
	* Отправляет данные на сервер (Хотя в названии и указано Replicated). 
	* В GAS много функций имеют такой суфикс независимо от того является ли это RPC or Replication
	*/ 
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	// Транслирует данные локально об успешном получении Target Data (положении курсора в нашем случае)
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	/*
	* Когда данные доходят до сервера, AbilitySystem сохранеет их в кеше
	* Кеш состоит из Map в котором хранится AbilitySpeckHandle и PredictionKey
	* Код ниже указывает, что он получил данные и не нужно их больше хранить - очисти кеш
	*/
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
