// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballGameMode.h"
#include "ChattingPlayerController.h"
#include "ChattingGameState.h"

void ABaseballGameMode::BeginPlay()
{
    Super::BeginPlay();
    GenerateRandomNumber();
    GameState = EGameState::InProgress;
}

void ABaseballGameMode::GenerateRandomNumber()
{
    // TSet을 이용해서 겹치지 않는 3자리 숫자 생성
    TSet<int32> UniqueNumbers;
    while (UniqueNumbers.Num() < 3)
    {
        int RandomNumber = FMath::RandRange(1, 9);
        UniqueNumbers.Add(RandomNumber);
    }
    // 생성된 숫자를 랜덤으로 결과에 삽입
    for (int32 Num : UniqueNumbers)
    {
        Answer += FString::Printf(TEXT("%d"), Num);
    }

    UE_LOG(LogTemp, Warning, TEXT("Generated Number: %s"), *Answer);
}

void ABaseballGameMode::CompareNumbers(const FString& Guess, int32& Strikes, int32& Balls)
{
    UE_LOG(LogTemp, Warning, TEXT("ABaseballGameMode : BeginPlay"));
    for (int32 i = 0; i < 3; i++)
    {
        for (int32 j = 0; j < 3; j++)
        {
            // 서로의 숫자가 일치할 경우
            if (Answer[i] == Guess[j])
            {
                // 만일 같은 위치라면 스트라이크 추가
                if (i == j)
                {
                    Strikes++;
                }
                // 다른 위치라면 볼 추가
                else
                {
                    Balls++;
                }
            }
        }
    }
}

void ABaseballGameMode::BroadcastResult(const FString& PlayerName, int32 Strikes, int32 Balls)
{
    AChattingGameState* GS = GetGameState<AChattingGameState>();
    if (GS)
    {
        GS->MulticastBroadcastResult(PlayerName, Strikes, Balls);
    }
}

void ABaseballGameMode::DeclareWinner(const FString& PlayerName)
{
    AChattingGameState* GS = GetGameState<AChattingGameState>();
    if (GS)
    {
        GS->MulticastDeclareWinner(PlayerName, Answer);
    }
}

void ABaseballGameMode::ProcessGuess_Implementation(const FString& Guess, const FString& PlayerName)
{
    if (GameState != EGameState::InProgress) return;

    int32 Strikes = 0, Balls = 0;
    CompareNumbers(Guess, Strikes, Balls);
    BroadcastResult(PlayerName, Strikes, Balls);

    if (Strikes == Answer.Len()) {
        DeclareWinner(PlayerName);
        GameState = EGameState::Finished;
    }
}
