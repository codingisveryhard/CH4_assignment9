// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballGameMode.h"
#include "ChattingPlayerController.h"
#include "ChattingGameState.h"
#include "BaseBallPlayerState.h"

void ABaseballGameMode::BeginPlay()
{
    Super::BeginPlay();

    BaseballGameState = EGameState::Finished;
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
        // 현재 턴 플레이어의 시도 횟수를 차감
        for (APlayerState* PlayerState : CurrentGameState->PlayerArray)
        {
            if (ABaseBallPlayerState* CurrentPlayerState = Cast<ABaseBallPlayerState>(PlayerState))
            {
                if (CurrentPlayerState->PlayerNumber == CurrentGameState->CurrentTurnPlayerNumber)
                {
                    CurrentPlayerState->UseTryCount();
                }
            }
        }

        if (IsDrawGame()) return;

        // 시간 만료 메시지 전송
        BroadcastSystemMessage(TEXT("[시스템] 시간 내 입력하지 않아, 턴을 넘깁니다."));

        // 타이머 정리 후 턴 종료 처리
        GetWorldTimerManager().ClearTimer(TurnTimerHandle);
        EndTurn();
    }
}

void ABaseballGameMode::EndTurn()
{
    AChattingGameState* CurrentGameState = GetGameState<AChattingGameState>();
    if (!CurrentGameState) return;

    // 다음 턴 플레이어 계산
    int32 CurrentIndex = CurrentGameState->CurrentTurnPlayerNumber;
    int32 NewIndex = CurrentIndex % CurrentGameState->PlayerArray.Num();
    int32 NewTurn = NewIndex + 1;
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
    AChattingGameState* CurrentGameState = GetGameState<AChattingGameState>();
    if (CurrentGameState)
    {
        CurrentGameState->MulticastBroadcastSystemMessage(SystemMessage);
    }
}

void ABaseballGameMode::RestartGame()
{
    if (BaseballGameState != EGameState::Finished) return;

    BroadcastSystemMessage((TEXT("[시스템] 게임을 시작합니다.")));
    GenerateRandomNumber();
    BaseballGameState = EGameState::InProgress;
    ResetAllPlayerTryCount();
    
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

bool ABaseballGameMode::IsEndGame(const FString& Guess, const int32& PlayerNumber)
{
    if (!IsValidGuess(Guess))
    {
        for (APlayerState* PlayerState : GameState->PlayerArray)
        {
            if (ABaseBallPlayerState* TargetPlayerState = Cast<ABaseBallPlayerState>(PlayerState))
            {
                if (TargetPlayerState->PlayerNumber != PlayerNumber)
                {
                    DeclareWinner(TargetPlayerState->PlayerNumber);
                }
            }
        }
        BaseballGameState = EGameState::Finished;
        GetWorldTimerManager().ClearTimer(TurnTimerHandle);
        return true;
    }

    return false;
}

bool ABaseballGameMode::IsDrawGame()
{
    AChattingGameState* ServerGameState = GetGameState<AChattingGameState>();

    if (!ServerGameState) return false;

    for (APlayerState* PlayerState : ServerGameState->PlayerArray)
    {
        if (ABaseBallPlayerState* TargetPlayerState = Cast<ABaseBallPlayerState>(PlayerState))
        {
            if (TargetPlayerState->GetTryCount() > 0)
            {
                return false;
            }
        }
    }

    BroadcastSystemMessage((TEXT("[시스템] 모든 플레이어가 턴을 모두 소진하여 무승부처리 하겠습니다.")));
    BaseballGameState = EGameState::Finished;
    GetWorldTimerManager().ClearTimer(TurnTimerHandle);
    return true;
}

bool ABaseballGameMode::IsValidGuess(const FString& Guess)
{
    if (Guess.Len() != 3 || !Guess.IsNumeric())
    {
        BroadcastSystemMessage((TEXT("[시스템] 잘못된 값 입력으로 Out처리 하겠습니다.")));
        return false;
    }

    TSet<TCHAR> UniqueChars;
    for (TCHAR c : Guess)
    {
        if (UniqueChars.Contains(c))
        {
            BroadcastSystemMessage((TEXT("[시스템] 잘못된 값 입력으로 Out처리 하겠습니다.")));
            return false;
        }
        UniqueChars.Add(c);
    }

    int32 Balls = 0;
    int32 Strikes = 0;
    for (int32 i = 0; i < 3; i++)
    {
        for (int32 j = 0; j < 3; j++)
        {
            if (Answer[i] == Guess[j])
            {
                if (i == j)
                {
                    Strikes++;
                }
                else
                {
                    Balls++;
                }
            }
        }
    }
    if (Strikes == 0 && Balls == 0)
    {
        BroadcastSystemMessage((TEXT("[시스템] 3 Out으로 즉시 패배했습니다.")));
        return false;
    }

    return true;
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

    if (IsEndGame(Guess, PlayerNumber)) return;

    int32 Strikes = 0, Balls = 0;
    CompareNumbers(Guess, Strikes, Balls);
    BroadcastResult(PlayerNumber, Strikes, Balls);

    if (Strikes == Answer.Len())
    {
        DeclareWinner(PlayerNumber);
        BaseballGameState = EGameState::Finished;
        return;
    }

    if (IsDrawGame()) return;
    

    int32 CurrentIndex = ServerGameState->CurrentTurnPlayerNumber;
    int32 NewIndex = CurrentIndex % ServerGameState->PlayerArray.Num();
    int32 NewTurn = NewIndex + 1;
    ServerGameState->MulticastUpdateTurn(NewTurn);

    StartTurnTimer();
}
