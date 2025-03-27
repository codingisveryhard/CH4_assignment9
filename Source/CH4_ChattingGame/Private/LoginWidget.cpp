// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include "BaseBallPlayerState.h"
#include "ChattingPlayerController.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"

void ULoginWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!LoginButton) return;

    LoginButton->OnClicked.AddDynamic(this, &ULoginWidget::OnLoginClicked);
}

void ULoginWidget::OnLoginClicked()
{
    if (!PlayerNameTextBox)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerNameTextBox is not bound!"));
        return;
    }

    FString InputPlayerName = PlayerNameTextBox->GetText().ToString();
    if (InputPlayerName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Input is empty!"));
        return;
    }

    AChattingPlayerController* PlayerController = GetOwningPlayer<AChattingPlayerController>();
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerController is NULL!"));
        return;
    }

    PlayerController->ServerSetPlayerNickname(InputPlayerName);
    PlayerController->ShowGameWidget();

}

void ULoginWidget::SetDefaultName(const FString& Name)
{
    if (PlayerNameTextBox)
    {
        PlayerNameTextBox->SetText(FText::FromString(Name));
    }
}
