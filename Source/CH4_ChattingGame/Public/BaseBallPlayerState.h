// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BaseBallPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerNicknameUpdated, const FString&, NewNickname);

UCLASS()
class CH4_CHATTINGGAME_API ABaseBallPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
    ABaseBallPlayerState();

    // 플레이어 넘버 할당
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
    int32 PlayerNumber;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
    FString PlayerNickname;

    // 남은 도전 기회
    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Baseball")
    int32 TryCount;

    // 승리 횟수
    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Baseball")
    int32 WinCount;

    // 남은 기회 감소 함수
    UFUNCTION(BlueprintCallable, Category = "Baseball")
    void UseTryCount();

    // 승리 카운트 증가 함수
    UFUNCTION(BlueprintCallable, Category = "Baseball")
    void AddWinCount();

    // 남은 기회 감소 함수
    UFUNCTION(BlueprintCallable, Category = "Baseball")
    void ResetTryCount();

    // 플레이어 이름 교체 함수
    UFUNCTION(BlueprintCallable, Category = "Baseball")
    void SetPlayerNickname(const FString& NewPlayerName);

    //// 디폴트 네임 할당 함수
    //UFUNCTION(BlueprintCallable, Category = "Player")
    //void SetDefaultNickname(const int32& NewPlayerNumber);

    UFUNCTION()
    void OnRep_PlayerNickname();

    UPROPERTY(BlueprintAssignable, Category = "Player")
    FOnPlayerNicknameUpdated OnPlayerNicknameUpdated;

    // 복제 속성 설정
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
