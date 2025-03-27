// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BaseballGameMode.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8 {
    Waiting,
    InProgress,
    Finished
};

UCLASS()
class CH4_CHATTINGGAME_API ABaseballGameMode : public AGameMode
{
	GENERATED_BODY()
    
protected:
    FTimerHandle TurnTimerHandle;

public:

    virtual void BeginPlay() override;

    void PostLogin(APlayerController* NewPlayer) override;

    // 
    UFUNCTION(Server, Reliable)
    void ServerProcessGuess(const FString& Guess, const int32& PlayerNumber);

    // 턴 시작 시 호출
    void StartTurnTimer();

    // 타이머 업데이트 함수
    void UpdateTurnTimer();

    void EndTurn();

private:
    // 서버에서 생성한 정답값
    FString Answer;
    // 현재 게임의 진행 상태를 표시하는 변수
    EGameState BaseballGameState;

    // 겹치지 않는 랜덤한 3자리 숫자를 생성하는 함수
    void GenerateRandomNumber();
    // 플레이어가 입력한 채팅 내용과 숫자를 비교하여 결과를 반환하는 함수
    void CompareNumbers(const FString& Guess, int32& Strikes, int32& Balls);
    // 
    void BroadcastResult(const int32& PlayerNumber, int32 Strikes, int32 Balls);
    void DeclareWinner(const int32& WinningPlayerNumber);

    void BroadcastSystemMessage(const FString& SystemMessage);

    void RestartGame();
    void ResetAllPlayerTryCount();

    bool IsThreeNumber();
    bool IsDrawGame();
    void IsWinGame();
};
