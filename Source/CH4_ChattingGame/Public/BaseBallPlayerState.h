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
	
private:


public:
    ABaseBallPlayerState();

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
    int32 PlayerNumber;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
    FString PlayerNickname;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Baseball")
    int32 WinCount;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Baseball")
    int32 TryCount;

    UFUNCTION(BlueprintCallable, Category = "Baseball")
    void UseTryCount();

    UFUNCTION(BlueprintCallable, Category = "Baseball")
    int32 GetTryCount();

    UFUNCTION(BlueprintCallable, Category = "Baseball")
    void ResetTryCount();

    UFUNCTION(BlueprintCallable, Category = "Baseball")
    void AddWinCount();


    UFUNCTION(BlueprintCallable, Category = "Baseball")
    void SetPlayerNickname(const FString& NewPlayerName);

    //// 디폴트 네임 할당 함수
    //UFUNCTION(BlueprintCallable, Category = "Player")
    //void SetDefaultNickname(const int32& NewPlayerNumber);

    UFUNCTION()
    void OnRep_PlayerNickname();

    UPROPERTY(BlueprintAssignable, Category = "Player")
    FOnPlayerNicknameUpdated OnPlayerNicknameUpdated;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
