// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseBallPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BASEBALLASSIGNMENT_API ABaseBallPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	UFUNCTION(Exec)
	void Guess(const FString& Input);
	
	void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerSubmitAnswer(const FString& Input);
	
};
