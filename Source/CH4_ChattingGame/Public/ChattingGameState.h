#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ChattingGameState.generated.h"

UCLASS()
class CH4_CHATTINGGAME_API AChattingGameState : public AGameState
{
    GENERATED_BODY()

public:
    // 채팅 관련
    UFUNCTION(NetMulticast, Reliable)
    void MulticastReceiveChatMessage(const FString& Message);



    // 야구 게임 관련
    UFUNCTION(NetMulticast, Reliable)
    void MulticastBroadcastResult(const FString& ClientName, const int32& Strikes, const int32& Balls);

    UFUNCTION(NetMulticast, Reliable)
    void MulticastDeclareWinner(const FString& Winner, const FString& CorrectAnswer);
};
