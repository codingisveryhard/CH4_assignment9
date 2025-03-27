// Fill out your copyright notice in the Description page of Project Settings.


#include "ChattingUserWidget.h"
#include "ChattingPlayerController.h"
#include "ChattingMessageData.h"
#include "BaseBallPlayerState.h"
#include "ScoreData.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/ListView.h"


void UChattingUserWidget::UpdateUIDisplay(float Seconds)
{
    int32 Minutes = FMath::FloorToInt(Seconds / 60);
    int32 RemainingSeconds = FMath::FloorToInt(Seconds) % 60;
    FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, RemainingSeconds);
    TimerText->SetText(FText::FromString(TimeString));

    UpdateNickname();
    UpdateScoreboard();
}

void UChattingUserWidget::RefreshTimerDisplay()
{
    if (AChattingGameState* CurrentGameState = GetWorld()->GetGameState<AChattingGameState>())
    {
        // GameState에서 타이머 값을 읽어와서 업데이트 (호스트, 게스트 모두 동일하게 동작)
        UpdateUIDisplay(CurrentGameState->RemainingTurnTime);
    }

    UpdateTurnColor();
}

void UChattingUserWidget::UpdateTurnColor()
{
    if (AChattingGameState* CurrentGameState = GetWorld()->GetGameState<AChattingGameState>())
    {
        if (AChattingPlayerController* CurrentPlayerController = GetOwningPlayer<AChattingPlayerController>())
        {
            if (ABaseBallPlayerState* CurrentPlayerState = CurrentPlayerController->GetPlayerState<ABaseBallPlayerState>())
            {
                if (CurrentPlayerState->PlayerNumber == CurrentGameState->CurrentTurnPlayerNumber)
                {
                    if (NameBorder)
                    {
                        NameBorder->SetBrushColor(FLinearColor::Green);
                    }
                }
                else
                {
                    if (NameBorder)
                    {
                        NameBorder->SetBrushColor(FLinearColor::White);
                    }
                }
            }
        }
    }
}

void UChattingUserWidget::UpdateNickname()
{
    if (AChattingPlayerController* PC = GetOwningPlayer<AChattingPlayerController>())
    {
        if (ABaseBallPlayerState* PS = PC->GetPlayerState<ABaseBallPlayerState>())
        {
            // 닉네임이 변경되었을 때만 업데이트
            if (PS->PlayerNickname != LastNicknameText)
            {
                LastNicknameText = PS->PlayerNickname;
                if (NicknameText)
                {
                    NicknameText->SetText(FText::FromString(LastNicknameText));
                }
            }
        }
    }
}

void UChattingUserWidget::UpdateScoreboard()
{
    AChattingGameState* GameState = GetWorld()->GetGameState<AChattingGameState>();
    if (!GameState) return;
    TArray<FPlayerScoreInfo> Scores = GameState->GetAllPlayerScores();

    if (!ScoreListView) return;

    ScoreListView->ClearListItems();

    for (const FPlayerScoreInfo& Score : Scores) {
        UScoreData* ScoreData = NewObject<UScoreData>();
        ScoreData->PlayerName = Score.PlayerName;
        ScoreData->Wins = Score.Wins;
        ScoreListView->AddItem(ScoreData);
    }
}

void UChattingUserWidget::OnPlayerNicknameUpdated(const FString& NewNickname)
{
    if (NicknameText)
    {
        NicknameText->SetText(FText::FromString(NewNickname));
        UE_LOG(LogTemp, Warning, TEXT("UChattingUserWidget updated nickname: %s"), *NewNickname);
    }
}

void UChattingUserWidget::AddChattingMessage(UChattingMessageData* NewChatData)
{
    if (NewChatData)
    {
        // 채팅 배열에 메시지를 추가
        AllChatMessages.Add(NewChatData);

        if (ChatListView)
        {
            ChatListView->AddItem(NewChatData);
            ChatListView->ScrollToBottom();
        }
    }
}

void UChattingUserWidget::OnChatSubmitted(const FText& Text, ETextCommit::Type CommitMethod)
{
    // 엔터 외의 입력일 시 무시
    if (CommitMethod != ETextCommit::OnEnter) return;

    if (CommitMethod == ETextCommit::OnEnter)
    {
        AChattingPlayerController* PC = GetOwningPlayer<AChattingPlayerController>();
        if (PC)
        {
            // 채팅에 전달
            PC->SendChatMessage(Text.ToString());
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

    ChatDisplay->SetText(FText::FromString(NewText));
}

void UChattingUserWidget::ResetChat()
{
    // 리스트 뷰 항목 삭제
    if (ChatListView)
    {
        ChatListView->ClearListItems();
        UE_LOG(LogTemp, Warning, TEXT("Chat list cleared"));
    }

    // 데이터 배열 초기화
    AllChatMessages.Empty();

    // 새게임 시작시 스코어를 알려주거나 새로 시작되었다는 문구를 추가 해보기
    // AddChattingMessage(NewChatData);
}

void UChattingUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ChatInputBox)
    {
        ChatInputBox->OnTextCommitted.AddDynamic(this, &UChattingUserWidget::OnChatSubmitted);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ChatInputBox is nullptr! Check UMG binding."));
    }

    if (AChattingPlayerController* CurrentPlayerController = GetOwningPlayer<AChattingPlayerController>())
    {
        if (ABaseBallPlayerState* CurrentPlayerState = CurrentPlayerController->GetPlayerState<ABaseBallPlayerState>())
        {
            CurrentPlayerState->OnPlayerNicknameUpdated.AddDynamic(this, &UChattingUserWidget::OnPlayerNicknameUpdated);
            if (NicknameText)
            {
                NicknameText->SetText(FText::FromString(CurrentPlayerState->PlayerNickname));
            }
        }
    }
    // 기존 코드
    //if (AChattingGameState* CurrentGameState = GetWorld()->GetGameState<AChattingGameState>())
    //{
    //    CurrentGameState->OnTurnTimeUpdated.AddDynamic(this, &UChattingUserWidget::UpdateTimerDisplay);
    //}

    // 수정된 코드
    GetWorld()->GetTimerManager().SetTimer(TimerUpdateHandle, this, &UChattingUserWidget::RefreshTimerDisplay, 0.1f, true);
}