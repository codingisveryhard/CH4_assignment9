// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballGameMode.h"
#include "ChattingPlayerController.h"
#include "ChattingGameState.h"
#include "BaseBallPlayerState.h"

void ABaseballGameMode::BeginPlay()
{
    Super::BeginPlay();
    GenerateRandomNumber();
    BaseballGameState = EGameState::InProgress;
}

void ABaseballGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    AChattingGameState* CurrentGameState = GetGameState<AChattingGameState>();

    if (!CurrentGameState) return;

    ABaseBallPlayerState* CurrentPlayerState = Cast<ABaseBallPlayerState>(NewPlayer->PlayerState);
    if (CurrentPlayerState)
    {
        CurrentPlayerState->PlayerNumber = CurrentGameState->PlayerArray.Num();

        // 초기 디폴트 네임 설정
        //CurrentPlayerState->SetDefaultNickname(CurrentPlayerState->PlayerNumber);
    }
}

void ABaseballGameMode::StartTurnTimer()
{
    AChattingGameState* CurrentGameState = GetGameState<AChattingGameState>();
    if (!CurrentGameState) return;

    // 타이머 초기화
    CurrentGameState->RemainingTurnTime = CurrentGameState->MaxTurnTime;
    CurrentGameState->MulticastUpdateTurnTime(CurrentGameState->RemainingTurnTime);

    // 1초 간격으로 타이머 업데이트
    GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ABaseballGameMode::UpdateTurnTimer, 1.f, true);
}

void ABaseballGameMode::UpdateTurnTimer()
{
    AChattingGameState* CurrentGameState = GetGameState<AChattingGameState>();
    if (!CurrentGameState) return;

    // 남은 시간 감소
    CurrentGameState->RemainingTurnTime -= 1.f;
    /*CurrentGameState->MulticastUpdateTurnTime(CurrentGameState->RemainingTurnTime);*/

    // 시간 종료 시 턴 넘김
    if (CurrentGameState->RemainingTurnTime <= 0)
    {
        GetWorldTimerManager().ClearTimer(TurnTimerHandle);
        EndTurn();
    }
}

void ABaseballGameMode::EndTurn()
{
    AChattingGameState* CurrentGameState = GetGameState<AChattingGameState>();
    if (!CurrentGameState) return;

    // 다음 턴 플레이어 계산
    int32 NewTurn = (CurrentGameState->CurrentTurnPlayerNumber == 1) ? 2 : 1;
    CurrentGameState->MulticastUpdateTurn(NewTurn);

    // 새 턴 타이머 시작
    StartTurnTimer();
}

void ABaseballGameMode::GenerateRandomNumber()
{
    Answer = "";
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

void ABaseballGameMode::BroadcastResult(const int32& PlayerNumber, int32 Strikes, int32 Balls)
{
    AChattingGameState* CurrentGameState = GetGameState<AChattingGameState>();
    if (CurrentGameState)
    {
        FString PlayerName = CurrentGameState->GetPlayerName(PlayerNumber);
        CurrentGameState->MulticastBroadcastResult(PlayerName, Strikes, Balls);
    }
}

void ABaseballGameMode::DeclareWinner(const int32& WinningPlayerNumber)
{
    AChattingGameState* CurrentGameState = GetGameState<AChattingGameState>();

    if (!CurrentGameState) return;
    // 모든 PlayerState를 순회하며 승리한 플레이어 찾기
    for (APlayerState* PS : CurrentGameState->PlayerArray)
    {
        ABaseBallPlayerState* PlayerState = Cast<ABaseBallPlayerState>(PS);
        if (PlayerState && PlayerState->PlayerNumber == WinningPlayerNumber)
        {
            // 승리 횟수 증가
            PlayerState->AddWinCount();

            FString WinnerName = CurrentGameState->GetPlayerName(WinningPlayerNumber);
            CurrentGameState->MulticastDeclareWinner(WinnerName, Answer);
            break;
        }
    }
    GetWorldTimerManager().ClearTimer(TurnTimerHandle);
}

void ABaseballGameMode::BroadcastSystemMessage(const FString& SystemMessage)
{
}

void ABaseballGameMode::RestartGame()
{
    GenerateRandomNumber();
    BaseballGameState = EGameState::InProgress;
    ResetAllPlayerTryCount();
    // 턴 초기화
    if (AChattingGameState* ServerGameState = GetGameState<AChattingGameState>())
    {
        ServerGameState->CurrentTurnPlayerNumber = 1;
        ServerGameState->MulticastUpdateTurn(1);
    }

    StartTurnTimer();
}

void ABaseballGameMode::ResetAllPlayerTryCount()
{
    AChattingGameState* CurrentGameState = GetGameState<AChattingGameState>();
    for (APlayerState* CurrentPlayerState : CurrentGameState->PlayerArray)
    {
        if (ABaseBallPlayerState* TargetPlayerState = Cast<ABaseBallPlayerState>(CurrentPlayerState))
        {
            TargetPlayerState->ResetTryCount();
        }
    }
}

bool ABaseballGameMode::IsDrawGame()
{
    return false;
}

void ABaseballGameMode::ServerProcessGuess_Implementation(const FString& Guess, const int32& PlayerNumber)
{
    AChattingGameState* ServerGameState = GetGameState<AChattingGameState>();
    if (!ServerGameState || BaseballGameState != EGameState::InProgress) return;

    if (PlayerNumber != ServerGameState->CurrentTurnPlayerNumber)
    {
        if (APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController())
        {
            PC->ClientMessage(FString::Printf(TEXT("당신의 차례가 아닙니다!")));
        }
        return;
    }

    ABaseBallPlayerState* PS = nullptr;
    for (APlayerState* PlayerState : ServerGameState->PlayerArray)
    {
        if (ABaseBallPlayerState* BPS = Cast<ABaseBallPlayerState>(PlayerState))
        {
            if (BPS->PlayerNumber == PlayerNumber && BPS->TryCount > 0)
            {
                PS = BPS;
                break;
            }
        }
    }
    if (!PS) return;

    int32 Strikes = 0, Balls = 0;
    CompareNumbers(Guess, Strikes, Balls);
    BroadcastResult(PlayerNumber, Strikes, Balls);

    if (Strikes == Answer.Len())
    {
        DeclareWinner(PlayerNumber);
        BaseballGameState = EGameState::Finished;
        RestartGame();
    }
    else
    {
        int32 CurrentIndex = ServerGameState->CurrentTurnPlayerNumber;
        int32 NewIndex = CurrentIndex % ServerGameState->PlayerArray.Num();
        int32 NewTurn = NewIndex + 1;
        ServerGameState->MulticastUpdateTurn(NewTurn);
    }

    StartTurnTimer();
}
