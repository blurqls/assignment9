// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseballGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BASEBALLASSIGNMENT_API ABaseballGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	ABaseballGameMode();
	bool ValidateInput(const FString& Input);
	FString CheckAnswer(const FString& Input);
	
	void HandleWin(APlayerController* Winner);
	void CheckDraw();
	void ResetGame();
	void BroadcastMessage(const FString& Message);
	void PostLogin(APlayerController* NewPlayer) override;
	void HandleGuess(APlayerController* PC,class ABaseballPlayerState* PS,const FString& Result);
	void HandleInvalidInput(APlayerController* PC, const FString& Reason);

protected:

	// 정답 숫자 저장
	UPROPERTY()
	TArray<int32> AnswerNumbers;

	// 랜덤 숫자 생성
	void GenerateRandomNumbers();
};
