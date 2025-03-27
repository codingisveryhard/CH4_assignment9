// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ScoreData.generated.h"

/**
 * 
 */
UCLASS()
class CH4_CHATTINGGAME_API UScoreData : public UObject
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadOnly)
    FString PlayerName;

    UPROPERTY(BlueprintReadOnly)
    int32 Wins;
};
