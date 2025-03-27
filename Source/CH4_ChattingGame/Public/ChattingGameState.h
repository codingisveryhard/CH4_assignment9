#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ChattingGameState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnTimeUpdated, float, NewTime);

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

    UFUNCTION(NetMulticast, Reliable)
    void MulticastBroadcastSystemMessage(const FString& SystemMessage);

    // 현재 턴 플레이어 번호
    UPROPERTY(Replicated, BlueprintReadOnly)
    int32 CurrentTurnPlayerNumber = 1;

    // 턴 변경 알림 함수
    UFUNCTION(NetMulticast, Reliable)
    void MulticastUpdateTurn(int32 NewTurnNumber);

    // 리플리케이트된 턴 시간 변수
    UPROPERTY(ReplicatedUsing = OnRep_RemainingTurnTime, BlueprintReadOnly)
    float RemainingTurnTime = 0.f;


    UPROPERTY(BlueprintAssignable, Category = "Turn")
    FOnTurnTimeUpdated OnTurnTimeUpdated;

    UFUNCTION()
    void OnRep_RemainingTurnTime();

    // 최대 턴 시간
    UPROPERTY(EditDefaultsOnly, Category = "Turn")
    float MaxTurnTime = 10.f;

    // 타이머 업데이트 알림 함수
    UFUNCTION(NetMulticast, Reliable)
    void MulticastUpdateTurnTime(float NewTime);


    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // 할당된 플레이어 넘버를 이용해 플레이어의 이름 반환
    FString GetPlayerName(const int32& PlayerNumber);

};
