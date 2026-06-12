#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BaseballGameState.generated.h"


UCLASS()
class BASEBALLASSIGNMENT_API ABaseballGameState : public AGameStateBase
{
	GENERATED_BODY()

public:



	UPROPERTY(Replicated)
	bool bGameEnded = false;

	UPROPERTY(Replicated)
	FString WinnerName;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastShowMessage(const FString& Message);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};