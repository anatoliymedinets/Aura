// Copyright AM Company

#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** IPlayerInterface **/
	virtual void AddToXP_Implementation(int32 InXP) override;
	/** end IPlayerInterface **/

	/** ICombatInterface */
	virtual int32 GetPlayerLevel() override;
	/** end ICombatInterface */

protected:
	virtual void InitAbilityActorInfo() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

	void SetupCamera();
	void SetupMovement();

};
