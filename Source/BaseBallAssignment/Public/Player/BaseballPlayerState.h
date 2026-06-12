// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BaseballPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BASEBALLASSIGNMENT_API ABaseballPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	ABaseballPlayerState();

	UPROPERTY(Replicated)
	int32 CurrentTry;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxTry;
	
	FString MakeStatusText() const
	{
		return FString::Printf(TEXT("[%d/%d]"), CurrentTry, MaxTry);
	}
	
	UPROPERTY(Replicated)
	bool bGameEnded = false;

	UPROPERTY(Replicated)
	FString WinnerName;
	
	UPROPERTY(Replicated)
	FString DisplayName;

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps
	) const override;

	void IncreaseTry();

	bool IsOutOfChance() const;
	
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentTry() const
	{
		return CurrentTry;
	}

	UFUNCTION(BlueprintCallable)
	int32 GetMaxTry() const
	{
		return MaxTry;
	}
};
