#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ChattingGameState.generated.h"

UCLASS()
class CH4_CHATTINGGAME_API AChattingGameState : public AGameState
{
    GENERATED_BODY()

public:
    UFUNCTION(NetMulticast, Reliable)
    void MulticastReceiveChatMessage(const FString& Message);
};
