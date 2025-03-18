// Fill out your copyright notice in the Description page of Project Settings.


#include "ChattingMessageData.h"

void UChattingMessageData::SetMessageText(const FText& NewText)
{
    if (!NewText.IsEmpty())
    {
        MessageText = NewText.ToString();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Attempted to set an empty or invalid FText."));
    }
}

FString UChattingMessageData::GetMessageText() const
{
	return MessageText;
}
