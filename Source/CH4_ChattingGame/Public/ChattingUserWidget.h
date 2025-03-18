// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChattingUserWidget.generated.h"

class UListView;
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

public:
	UPROPERTY(meta = (BindWidget))
	UListView* ChatListView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat")
	UChattingMessageData* ChatData;


	UFUNCTION(BlueprintCallable)
	void AddChattingMessage(UChattingMessageData* NewChatData);

	// 이후 필터용으로 사용 예정
	//UFUNCTION(BlueprintCallable)
	//void FilterChatMessages();



    UFUNCTION()
    void OnChatSubmitted(const FText& Text, ETextCommit::Type CommitMethod);

    void DisplayChatMessage(const FString& Message);

    virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TArray<UChattingMessageData*> AllChatMessages;
};