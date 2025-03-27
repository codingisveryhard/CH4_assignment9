// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ScoreWidget.generated.h"

class UTextBlock;

UCLASS()
class CH4_CHATTINGGAME_API UScoreWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WinsCountText;

	UFUNCTION(BlueprintCallable, Category = "ScoreEntry")
	void SetScoreEntryData(const FString& InPlayerName, int32 InWins);

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
