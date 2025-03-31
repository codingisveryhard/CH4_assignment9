// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBallPlayerState.h"
#include "ChattingGameState.h"
#include "Net/UnrealNetwork.h"


ABaseBallPlayerState::ABaseBallPlayerState()
{
    PlayerNickname = TEXT("Unknown");
    PlayerNumber = 0;
    TryCount = 3;
    WinCount = 0;
}

void ABaseBallPlayerState::UseTryCount()
{
    if (HasAuthority())
    {
        TryCount--;
        UE_LOG(LogTemp, Warning, TEXT("UseTryCount!"));
    }
}

int32 ABaseBallPlayerState::GetTryCount()
{
    return TryCount;
}

void ABaseBallPlayerState::AddWinCount()
{
    if (HasAuthority())
    {
        WinCount++;
    }
}

void ABaseBallPlayerState::ResetTryCount()
{
    if (HasAuthority())
    {
        TryCount = 3;
    }
}

void ABaseBallPlayerState::SetPlayerNickname(const FString& NewPlayerName)
{
    if (HasAuthority())
    {
        PlayerNickname = NewPlayerName;
        OnRep_PlayerNickname();
    }
}
//void ABaseBallPlayerState::SetDefaultNickname(const int32& NewPlayerNumber)
//{
//    if (HasAuthority())
//    {
//        // GameState 참조
//        UWorld* World = GetWorld();
//        if (!World) return;
//
//        AChattingGameState* GameState = World->GetGameState<AChattingGameState>();
//        if (GameState)
//        {
//            PlayerNickname = GameState->GetPlayerName(NewPlayerNumber);
//        }
//    }
//}

void ABaseBallPlayerState::OnRep_PlayerNickname()
{
    OnPlayerNicknameUpdated.Broadcast(PlayerNickname);
}

void ABaseBallPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABaseBallPlayerState, PlayerNumber);
    DOREPLIFETIME(ABaseBallPlayerState, PlayerNickname);
    DOREPLIFETIME(ABaseBallPlayerState, TryCount);
    DOREPLIFETIME(ABaseBallPlayerState, WinCount);
}