// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BaseballPlayerState.h"
#include "Net/UnrealNetwork.h"

ABaseballPlayerState::ABaseballPlayerState()
{
	CurrentTry = 0;
	MaxTry = 3;
	
	DisplayName = FString::Printf(TEXT("Player %d"), GetPlayerId());
}

void ABaseballPlayerState::IncreaseTry()
{
	CurrentTry++;
}

bool ABaseballPlayerState::IsOutOfChance() const
{
	return CurrentTry >= MaxTry;
}

void ABaseballPlayerState::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps
) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseballPlayerState, CurrentTry);
	DOREPLIFETIME(ABaseballPlayerState, DisplayName);
}
