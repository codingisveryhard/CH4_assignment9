// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreWidget.h"
#include "ScoreData.h"
#include "Components/TextBlock.h"

void UScoreWidget::SetScoreEntryData(const FString& InPlayerName, int32 InWins)
{
	if (PlayerNameText)
	{
		PlayerNameText->SetText(FText::FromString(InPlayerName));
	}
	if (WinsCountText)
	{
		WinsCountText->SetText(FText::AsNumber(InWins));
	}
}

void UScoreWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	if (UScoreData* ScoreData = Cast<UScoreData>(ListItemObject))
	{
		SetScoreEntryData(ScoreData->PlayerName, ScoreData->Wins);
	}
}
