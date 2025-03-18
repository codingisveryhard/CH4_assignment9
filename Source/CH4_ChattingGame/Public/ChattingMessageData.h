// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ChattingMessageData.generated.h"

/**
 * 
 */
UCLASS()
class CH4_CHATTINGGAME_API UChattingMessageData : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat")
	FString MessageText;

	void SetMessageText(const FText& NewText);
	FString GetMessageText() const;
};
