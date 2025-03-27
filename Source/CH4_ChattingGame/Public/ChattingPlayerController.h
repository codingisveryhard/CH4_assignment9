// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChattingUserWidget.h"
#include "LoginWidget.h"
#include "ChattingPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CH4_CHATTINGGAME_API AChattingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UChattingUserWidget* ChatUI;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    ULoginWidget* LoginUI;

    // 위젯 반환 getter
    UChattingUserWidget* GetChatWidget() const { return ChatUI; }

    virtual void BeginPlay() override;

    // 채팅 메시지를 서버에 전송하는 함수
    UFUNCTION(BlueprintCallable, Category = "Chat")
    void SendChatMessage(const FString& Message);

    // 클라이언트에서 호출하는 서버 RPC 함수
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerSendChatMessage(const int32& PlayerNumber, const FString& Message);

    // Chat UI에 접근할 수 있는 Getter
    UFUNCTION(BlueprintCallable)
    UChattingUserWidget* GetChatUI() const { return ChatUI; }

    void SetupUI();


    //////////////////////////////////////////////////////////
    // 야구 게임 관련
    // 야구 게임 메시지를 서버에 전송하는 함수
    UFUNCTION(BlueprintCallable, Category = "Chat")
    void SendGuessMessage(const FString& GuessNumber);

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerSendGuessMessage(const int32& PlayerNumber, const FString& GuessNumber);




    // 위젯 참조용 변수
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<ULoginWidget> LoginWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UChattingUserWidget> GameWidgetClass;

    // 위젯 전환용 함수
    UFUNCTION(BlueprintCallable)
    void ShowLoginWidget();

    UFUNCTION(BlueprintCallable)
    void ShowGameWidget();


    UFUNCTION(Server, Reliable, WithValidation)
    void ServerSetPlayerNickname(const FString& NewNickname);
};