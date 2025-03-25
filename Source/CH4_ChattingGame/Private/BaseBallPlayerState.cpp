// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBallPlayerState.h"
#include "Net/UnrealNetwork.h"


ABaseBallPlayerState::ABaseBallPlayerState()
{
    PlayerNumber = 0;
    TryCount = 3;
    WinCount = 0;
}

void ABaseBallPlayerState::UseTry()
{
    if (HasAuthority() && TryCount > 0)
    {
        TryCount--;
        UE_LOG(LogTemp, Warning, TEXT("ABaseBallPlayerState : UseTry : Remain TryCount : %d"), TryCount);
    }
}

void ABaseBallPlayerState::AddWin()
{
    if (HasAuthority())
    {
        WinCount++;
        // 기존 Score도 함께 증가시킬 수 있음
        SetScore(GetScore() + 1);
    }
}

void ABaseBallPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABaseBallPlayerState, PlayerNumber);
    DOREPLIFETIME(ABaseBallPlayerState, TryCount);
    DOREPLIFETIME(ABaseBallPlayerState, WinCount);
}