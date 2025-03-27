// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

class UEditableTextBox;
class UButton;

UCLASS()
class CH4_CHATTINGGAME_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void SetDefaultName(const FString& Name);


protected:
    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* PlayerNameTextBox;

    UPROPERTY(meta = (BindWidget))
    UButton* LoginButton;

    UFUNCTION()
    void OnLoginClicked();

    virtual void NativeConstruct() override;
};
