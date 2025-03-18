// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ChattingMessageWidget.generated.h"

class UTextBlock;

UCLASS()
class CH4_CHATTINGGAME_API UChattingMessageWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MessageTextBlock;

	// 메시지를 받아서 텍스트 블록에 설정하는 함수
	UFUNCTION(BlueprintCallable)
	void SetMessageText(const FString& InputMessageText);

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
