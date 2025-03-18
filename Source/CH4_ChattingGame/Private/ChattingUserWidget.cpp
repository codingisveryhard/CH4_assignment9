// Fill out your copyright notice in the Description page of Project Settings.


#include "ChattingUserWidget.h"
#include "ChattingPlayerController.h"
#include "ChattingMessageData.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"


void UChattingUserWidget::AddChattingMessage(UChattingMessageData* NewChatData)
{
    if (NewChatData)
    {
        // 채팅 배열에 메시지를 추가
        AllChatMessages.Add(NewChatData);

        if (ChatListView)
        {
            ChatListView->AddItem(NewChatData);
            // 리스트 뷰의 스크롤을 아래쪽으로 이동
            ChatListView->ScrollToBottom();
        }
    }
}

void UChattingUserWidget::OnChatSubmitted(const FText& Text, ETextCommit::Type CommitMethod)
{
    // 엔터 외의 입력일 시 무시
    if (CommitMethod != ETextCommit::OnEnter)
    {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("OnChatSubmitted called: %s, CommitMethod: %d"), *Text.ToString(), (int32)CommitMethod);

    if (CommitMethod == ETextCommit::OnEnter)
    {
        AChattingPlayerController* PC = GetOwningPlayer<AChattingPlayerController>();
        if (PC)
        {
            // 채팅에 전달
            PC->SendChatMessage(Text.ToString());
            UE_LOG(LogTemp, Warning, TEXT("SendChatMessage called with: %s"), *Text.ToString());
            // 야구 게임에 전달
            PC->SendGuessMessage(Text.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No valid PlayerController found"));
        }
        ChatInputBox->SetText(FText::GetEmpty());
    }
}

void UChattingUserWidget::DisplayChatMessage(const FString& Message)
{
    if (!ChatDisplay)
    {
        UE_LOG(LogTemp, Error, TEXT("ChatDisplay is nullptr! Check if it's correctly assigned in the UMG."));
        return;
    }

    FString CurrentText = ChatDisplay->GetText().ToString();
    FString NewText = CurrentText + "\n" + Message;

    UE_LOG(LogTemp, Warning, TEXT("Updating ChatDisplay: %s"), *NewText);

    ChatDisplay->SetText(FText::FromString(NewText));
}

void UChattingUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ChatInputBox)
    {
        ChatInputBox->OnTextCommitted.AddDynamic(this, &UChattingUserWidget::OnChatSubmitted);
        UE_LOG(LogTemp, Warning, TEXT("OnTextCommitted event bound to OnChatSubmitted"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ChatInputBox is nullptr! Check UMG binding."));
    }
}