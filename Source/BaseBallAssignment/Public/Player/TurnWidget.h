// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurnWidget.generated.h"

/**
 * 
 */
UCLASS()
class BASEBALLASSIGNMENT_API UTurnWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimeText;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
};
