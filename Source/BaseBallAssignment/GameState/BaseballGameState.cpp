// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballGameState.h"
#include "Net/UnrealNetwork.h"

void ABaseballGameState::MulticastShowMessage_Implementation(
	const FString& Message)
{
	static int32 Count = 0;
	Count++;

	ENetMode NetMode = GetNetMode();
	ENetRole LocalRole = GetLocalRole();

	FString DebugText = FString::Printf(
		TEXT("[Count:%d] [NetMode:%d] [Role:%d] %s"),
		Count,
		(int32)NetMode,
		(int32)LocalRole,
		*Message
	);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugText);

	if (GetNetMode() == NM_Client)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Yellow,
			DebugText
		);
	}
}

void ABaseballGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseballGameState, bGameEnded);
	DOREPLIFETIME(ABaseballGameState, WinnerName);
}