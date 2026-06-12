// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Player/BaseBallPlayerController.h"
#include "BaseBallAssignment/Public/GameMode/BaseballGameMode.h"
#include "Player/BaseballPlayerState.h"

void ABaseBallPlayerController::Guess(const FString& Input)
{
	ServerSubmitAnswer(Input);
}

void ABaseBallPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	switch(GetNetMode())
    {
    case NM_Client:
        UE_LOG(LogTemp, Warning,
            TEXT("CLIENT"));
        break;
    
    case NM_ListenServer:
        UE_LOG(LogTemp, Warning,
            TEXT("LISTEN SERVER"));
        break;
    }
    
    UE_LOG(LogTemp, Warning,
        TEXT("Role : %d"),
        (int32)GetLocalRole());
}



void ABaseBallPlayerController::ServerSubmitAnswer_Implementation(const FString& Input)
{
	ABaseballGameMode* GM = GetWorld()->GetAuthGameMode<ABaseballGameMode>();
	if (!GM) return;

	ABaseballPlayerState* PS = GetPlayerState<ABaseballPlayerState>();
	if (!PS) return;
	
	if (!GM->ValidateInput(Input))
	{
		GM->HandleInvalidInput(this, TEXT("잘못된 입력입니다."));
		return;
	}
	
	if (PS->IsOutOfChance())
	{
		GM->HandleInvalidInput(this, TEXT("기회를 모두 사용했습니다."));
		return;
	}

	PS->IncreaseTry();

	FString Result = GM->CheckAnswer(Input);
	GM->HandleGuess(this, PS, Result);
}