// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChattingGameState.h"
#include "ChattingUserWidget.generated.h"

class UListView;
class UBorder;
class UChattingMessageData;

UCLASS()
class CH4_CHATTINGGAME_API UChattingUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* ChatInputBox;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ChatDisplay;

	UPROPERTY(meta = (BindWidget))
	UListView* ChatListView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat")
	UChattingMessageData* ChatData;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NicknameText;

	UPROPERTY(meta = (BindWidget))
	UBorder* NameBorder;

	UPROPERTY(meta = (BindWidget))
	UListView* ScoreListView;

	UFUNCTION(BlueprintCallable)
	void AddChattingMessage(UChattingMessageData* NewChatData);


    UFUNCTION()
    void OnChatSubmitted(const FText& Text, ETextCommit::Type CommitMethod);

    void DisplayChatMessage(const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "Chat")
	void ResetChat();

	UFUNCTION(BlueprintCallable)
	void UpdateScoreboard();

    virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TArray<UChattingMessageData*> AllChatMessages;

private:
	UFUNCTION(BlueprintCallable)
	void UpdateUIDisplay(float Seconds);

	FTimerHandle TimerUpdateHandle;

	FString LastNicknameText;

	UFUNCTION()
	void RefreshTimerDisplay();

	void UpdateTurnColor();

	void UpdateNickname();

	UFUNCTION()
	void OnPlayerNicknameUpdated(const FString& NewNickname);
};