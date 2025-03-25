// Fill out your copyright notice in the Description page of Project Settings.


#include "ChattingGameState.h"
#include "ChattingPlayerController.h"
#include "ChattingUserWidget.h"
#include "ChattingMessageData.h"
#include "BaseBallPlayerState.h"
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
            UChattingUserWidget* ChatUI = PC->GetChatUI();
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


void AChattingGameState::MulticastBroadcastResult_Implementation(const FString& ClientName, const int32& Strikes, const int32& Balls)
{
    // 클라이언트에서 결과를 표시
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AChattingPlayerController* PC = Cast<AChattingPlayerController>(It->Get());
        if (PC && PC->ChatUI)
        {
            UChattingMessageData* NewChatData = NewObject<UChattingMessageData>(PC);
            if (NewChatData)
            {
                FString ResultMessage = FString::Printf(TEXT("%s님의 예측 결과: S: %d, B: %d, O: %d"), *ClientName, Strikes, Balls, 3 - Strikes - Balls);
                // 메시지 텍스트 설정
                NewChatData->SetMessageText(FText::FromString(ResultMessage));
                // 채팅 UI의 리스트 뷰에 추가
                PC->GetChatUI()->AddChattingMessage(NewChatData);
            }
        }
    }
}

void AChattingGameState::MulticastDeclareWinner_Implementation(const FString& Winner, const FString& CorrectAnswer)
{
    // 모든 클라이언트에 승자 알림
    FString WinnerMessage = FString::Printf(TEXT("%s님이 승리했습니다! 정답: %s"), *Winner, *CorrectAnswer);

    // 모든 플레이어 컨트롤러에게 메시지 전송
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AChattingPlayerController* PC = Cast<AChattingPlayerController>(It->Get());
        if (PC && PC->ChatUI)
        {
            UChattingMessageData* NewChatData = NewObject<UChattingMessageData>(PC);
            if (NewChatData)
            {
                NewChatData->SetMessageText(FText::FromString(WinnerMessage));
                PC->ChatUI->AddChattingMessage(NewChatData);
            }
        }
    }
}

FString AChattingGameState::GetPlayerName(const int32& PlayerNumber)
{
    switch (PlayerNumber)
    {
    case 0:
        return TEXT("Default");
    case 1:
        return TEXT("Host");
    case 2:
        return TEXT("Guest");
    default:
        return FString::Printf(TEXT("Player %d"), PlayerNumber);
    }
}
