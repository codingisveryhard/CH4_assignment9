// Fill out your copyright notice in the Description page of Project Settings.


#include "ChattingMessageWidget.h"
#include "ChattingMessageData.h"
#include "Components/TextBlock.h"


void UChattingMessageWidget::SetMessageText(const FString& InputMessageText)
{
	if (MessageTextBlock)
	{
		MessageTextBlock->SetText(FText::FromString(InputMessageText));
	}
}

void UChattingMessageWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

    // ListItemObject를 ChattingMessageData로 캐스팅
    UChattingMessageData* MessageData = Cast<UChattingMessageData>(ListItemObject);
    if (MessageData)
    {
        // MessageData에서 메시지 텍스트를 가져와 위젯에 설정
        SetMessageText(MessageData->GetMessageText());
    }
}
