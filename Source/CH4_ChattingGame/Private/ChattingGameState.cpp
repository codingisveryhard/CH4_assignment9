// Fill out your copyright notice in the Description page of Project Settings.


#include "ChattingGameState.h"
#include "ChattingPlayerController.h"
#include "ChattingUserWidget.h"
#include "ChattingMessageData.h"
#include "GameFramework/HUD.h"
#include "Net/UnrealNetwork.h"


void AChattingGameState::MulticastReceiveChatMessage_Implementation(const FString& Message)
{
    UE_LOG(LogTemp, Log, TEXT("Chat Message: %s"), *Message);

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AChattingPlayerController* PC = Cast<AChattingPlayerController>(It->Get());
        if (PC && PC->IsLocalController())
        {
            UChattingUserWidget* ChatUI = PC->GetChatWidget();
            if (ChatUI)
            {
                ChatUI->DisplayChatMessage(Message);
            }

            // 각 클라이언트마다 새로운 채팅 데이터 객체 생성 후 UI 업데이트
            UChattingMessageData* NewChatData = NewObject<UChattingMessageData>(PC);
            if (NewChatData)
            {
                // 메시지 텍스트 설정
                NewChatData->SetMessageText(FText::FromString(Message));
                // 채팅 UI의 리스트 뷰에 추가
                PC->GetChatUI()->AddChattingMessage(NewChatData);
            }

        }
    }
}