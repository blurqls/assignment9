// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BaseballGameMode.h"
#include "GameMode/Player/BaseBallPlayerController.h"
#include "Player/BaseballPlayerState.h"
#include "BaseBallAssignment/GameState/BaseballGameState.h"

void ABaseballGameMode::BeginPlay()
{
	Super::BeginPlay();

	GenerateRandomNumbers();
}

ABaseballGameMode::ABaseballGameMode()
{
	PlayerControllerClass = ABaseBallPlayerController::StaticClass();

	PlayerStateClass = ABaseballPlayerState::StaticClass();

	GameStateClass = ABaseballGameState::StaticClass();
}

bool ABaseballGameMode::ValidateInput(const FString& Input)
{
	if (Input.Len() != 3)
	{
		return false;
	}

	TSet<TCHAR> UniqueNumbers;

	for (TCHAR Ch : Input)
	{
		if (!FChar::IsDigit(Ch))
		{
			return false;
		}

		UniqueNumbers.Add(Ch);
	}

	return UniqueNumbers.Num() == 3;
}

FString ABaseballGameMode::CheckAnswer(const FString& Input)
{
	int32 Strike = 0;
	int32 Ball = 0;

	for (int32 i = 0; i < 3; i++)
	{
		for (int32 j = 0; j < 3; j++)
		{
			if (Input[i] == TCHAR('0' + AnswerNumbers[j]))
			{
				if (i == j)
				{
					Strike++;
				}
				else
				{
					Ball++;
				}
			}
		}
	}

	if (Strike == 0 && Ball == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(
		TEXT("%dS%dB"),
		Strike,
		Ball);
}

void ABaseballGameMode::HandleWin(APlayerController* Winner)
{
	ABaseballGameState* GS =
		GetGameState<ABaseballGameState>();

	if (!GS)
	{
		return;
	}

	GS->bGameEnded = true;

	ABaseballPlayerState* PS =
		Winner->GetPlayerState<ABaseballPlayerState>();

	FString WinnerDisplayName = TEXT("Unknown");

	if (PS)
	{
		WinnerDisplayName = PS->DisplayName;
	}

	GS->WinnerName = WinnerDisplayName;

	BroadcastMessage(
		FString::Printf(
			TEXT("%s Wins!"),
			*WinnerDisplayName));

	FTimerHandle TimerHandle;

	GetWorldTimerManager().SetTimer(
		TimerHandle,
		this,
		&ABaseballGameMode::ResetGame,
		5.f,
		false);
}
void ABaseballGameMode::CheckDraw()
{
	bool bAllUsed = true;

	for (APlayerState* PS : GameState->PlayerArray)
	{
		ABaseballPlayerState* BaseballPS =
			Cast<ABaseballPlayerState>(PS);

		if (BaseballPS &&
			!BaseballPS->IsOutOfChance())
		{
			bAllUsed = false;
			break;
		}
	}

	if (bAllUsed)
	{
		BroadcastMessage(TEXT("DRAW"));
	}
}

void ABaseballGameMode::ResetGame()
{
	GenerateRandomNumbers();

	for (APlayerState* PS : GameState->PlayerArray)
	{
		ABaseballPlayerState* BaseballPS =
			Cast<ABaseballPlayerState>(PS);

		if (BaseballPS)
		{
			BaseballPS->CurrentTry = 0;
		}
	}

	BroadcastMessage(TEXT("Game Reset"));
}


void ABaseballGameMode::BroadcastMessage(
	const FString& Message)
{
	ABaseballGameState* GS =
		GetGameState<ABaseballGameState>();

	if (!GS)
	{
		return;
	}

	GS->MulticastShowMessage(Message);
}

void ABaseballGameMode::GenerateRandomNumbers()
{
	AnswerNumbers.Empty();

	while (AnswerNumbers.Num() < 3)
	{
		int32 RandomNumber = FMath::RandRange(1, 9);

		if (!AnswerNumbers.Contains(RandomNumber))
		{
			AnswerNumbers.Add(RandomNumber);
			UE_LOG(LogTemp, Display, TEXT("생성된 랜덤 숫자 : %d"), RandomNumber);
		}
	}

	UE_LOG(LogTemp, Warning,
		TEXT("Answer : %d %d %d"),
		AnswerNumbers[0],
		AnswerNumbers[1],
		AnswerNumbers[2]);
}

void ABaseballGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ABaseballPlayerState* PS =
		NewPlayer->GetPlayerState<ABaseballPlayerState>();

	if (!PS) return;

	static int32 Index = 0;
	Index++;

	PS->DisplayName = FString::Printf(TEXT("Player %d"), Index);
}

void ABaseballGameMode::HandleGuess(APlayerController* PC, ABaseballPlayerState* PS, const FString& Result)
{
	if (!PS || !PC) return;

	FString FullMessage = FString::Printf(
		TEXT("%s : %s %s"),
		*PS->DisplayName,
		*Result,
		*PS->MakeStatusText()
	);

	BroadcastMessage(FullMessage);

	UE_LOG(LogTemp, Warning,
		TEXT("[%d/%d] %s"),
		PS->GetCurrentTry(),
		PS->GetMaxTry(),
		*Result);

	if (Result == TEXT("3S0B"))
	{
		HandleWin(PC);
		return;
	}

	CheckDraw();
}

void ABaseballGameMode::HandleInvalidInput(APlayerController* PC, const FString& Reason)
{
	if (!PC) return;

	FString Name = TEXT("Unknown");

	if (ABaseballPlayerState* PS = PC->GetPlayerState<ABaseballPlayerState>())
	{
		Name = PS->DisplayName;
	}

	FString Message = FString::Printf(
		TEXT("%s : %s"),
		*Name,
		*Reason
	);

	BroadcastMessage(Message);
}
