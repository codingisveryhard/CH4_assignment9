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
	
public:
    virtual void BeginPlay() override;

    UFUNCTION(Server, Reliable)
    void ProcessGuess(const FString& Guess, const FString& PlayerName);

private:
    FString Answer; // 예: "1234"
    EGameState GameState;

    void GenerateRandomNumber();
    void CompareNumbers(const FString& Guess, int32& Strikes, int32& Balls);
    void BroadcastResult(const FString& PlayerName, int32 Strikes, int32 Balls);
    void DeclareWinner(const FString& PlayerName);
};
