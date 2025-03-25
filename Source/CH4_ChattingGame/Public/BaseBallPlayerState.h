// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BaseBallPlayerState.generated.h"

UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
    None,
    Host,
    Guest
};

UCLASS()
class CH4_CHATTINGGAME_API ABaseBallPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
    ABaseBallPlayerState();

    // 플레이어의 이름
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
    int32 PlayerNumber;

    UPROPERTY(Replicated)
    EPlayerRole PlayerRole = EPlayerRole::None;

    // 남은 도전 기회
    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Baseball")
    int32 TryCount;

    // 승리 횟수
    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Baseball")
    int32 WinCount;

    // 남은 기회 감소 함수
    UFUNCTION(BlueprintCallable, Category = "Baseball")
    void UseTry();

    // 승리 카운트 증가 함수
    UFUNCTION(BlueprintCallable, Category = "Baseball")
    void AddWin();


    // 복제 속성 설정
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
