#include "ChattingPlayerController.h"
#include "BaseballGameMode.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "ChattingGameState.h"
#include "BaseBallPlayerState.h"

void AChattingPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 로컬 플레이어 컨트롤러인지 확인
    if (!IsLocalController())
    {
        UE_LOG(LogTemp, Warning, TEXT("Not a local controller. Skipping widget creation."));
        return;
    }

    ShowLoginWidget();
}

void AChattingPlayerController::SendChatMessage(const FString& Message)
{
    ABaseBallPlayerState* CurrentPlayerState = GetPlayerState<ABaseBallPlayerState>();

    if (!CurrentPlayerState) return;

    UE_LOG(LogTemp, Warning, TEXT("AChattingPlayerController::SendChatMessage called with: %s"), *Message);

    ServerSendChatMessage(CurrentPlayerState->PlayerNumber, Message);
}

void AChattingPlayerController::SetupUI()
{
    FInputModeUIOnly InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    SetInputMode(InputMode);
    bShowMouseCursor = true;
}

void AChattingPlayerController::SendGuessMessage(const FString& GuessNumber)
{    
    ABaseBallPlayerState* CurrentPlayerState = GetPlayerState<ABaseBallPlayerState>();

    if (!CurrentPlayerState) return;

    ServerSendGuessMessage(CurrentPlayerState->PlayerNumber, GuessNumber);
}

void AChattingPlayerController::ShowLoginWidget()
{
    if (!LoginUI && LoginWidgetClass)
    {
        LoginUI = CreateWidget<ULoginWidget>(this, LoginWidgetClass);
    }

    if (LoginUI)
    {
        // 초기 로그인 창에 떠오를 기본 닉네임
        //ABaseBallPlayerState* CurrentPlayerState = GetPlayerState<ABaseBallPlayerState>();
        //if (CurrentPlayerState)
        //{
        //    LoginUI->SetDefaultName(CurrentPlayerState->PlayerNickname);
        //}
        // 기존 위젯 제거
        if (ChatUI) ChatUI->RemoveFromParent();

        // 로그인 위젯 표시
        LoginUI->AddToViewport();
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}

void AChattingPlayerController::ShowGameWidget()
{
    if (!ChatUI && GameWidgetClass)
    {
        ChatUI = CreateWidget<UChattingUserWidget>(this, GameWidgetClass);
    }

    if (ChatUI)
    {
        // 로그인 위젯 제거
        if (LoginUI) LoginUI->RemoveFromParent();

        // 게임 위젯 표시
        ChatUI->AddToViewport();
        SetInputMode(FInputModeGameAndUI());
        bShowMouseCursor = true; // 필요에 따라 조절
    }
}

void AChattingPlayerController::ServerSetPlayerNickname_Implementation(const FString& NewNickname)
{
    if (ABaseBallPlayerState* PS = GetPlayerState<ABaseBallPlayerState>())
    {
        PS->SetPlayerNickname(NewNickname);
    }
}

bool AChattingPlayerController::ServerSetPlayerNickname_Validate(const FString& NewNickname)
{
    return true;
}

bool AChattingPlayerController::ServerSendGuessMessage_Validate(const int32& PlayerNumber, const FString& GuessNumber)
{
    //// 입력된 문자의 길이가 3자리가 아니면 취소
    //if (GuessNumber.Len() != 3) return false;

    //// 입력된 문자가 숫자가 아닐 시 취소
    //TSet<TCHAR> UniqueChars;
    //for (int32 i = 0; i < 3; i++)
    //{
    //    if (!FChar::IsDigit(GuessNumber[i])) return false;
    //    UniqueChars.Add(GuessNumber[i]);
    //}
    //// 입력된 문자가 중복될 시 취소
    //return UniqueChars.Num() == 3;
    return true;
}

void AChattingPlayerController::ServerSendGuessMessage_Implementation(const int32& PlayerNumber, const FString& GuessNumber)
{
    // 3자리 숫자인지 확인하고 게임 로직 처리
    if (GuessNumber.Len() == 3 && GuessNumber.IsNumeric())
    {
        // 플레이어 상태 확인
        ABaseBallPlayerState* PS = GetPlayerState<ABaseBallPlayerState>();
        if (PS && PS->TryCount > 0)
        {
            // 게임 모드에 추측 전달
            ABaseballGameMode* BaseballGM = Cast<ABaseballGameMode>(GetWorld()->GetAuthGameMode());
            if (BaseballGM)
            {
                BaseballGM->ServerProcessGuess(GuessNumber, PlayerNumber);
            }
        }
        else if (PS && PS->TryCount <= 0)
        {
            // 시도 횟수가 없을 때 메시지 표시
            AChattingGameState* ChatGameState = GetWorld()->GetGameState<AChattingGameState>();
            if (ChatGameState)
            {
                ChatGameState->MulticastReceiveChatMessage(TEXT("시스템: 남은 시도 횟수가 없습니다."));
            }
        }
    }
}

void AChattingPlayerController::ServerSendChatMessage_Implementation(const int32& PlayerNumber, const FString& Message)
{
    AChattingGameState* ChatGameState = GetWorld()->GetGameState<AChattingGameState>();
    if (ChatGameState)
    {
        
        FString FormattedMessage = ChatGameState->GetPlayerName(PlayerNumber) + ": " + Message;
        ChatGameState->MulticastReceiveChatMessage(FormattedMessage);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ChatGameState is nullptr"));
    }
}

bool AChattingPlayerController::ServerSendChatMessage_Validate(const int32& PlayerNumber, const FString& Message)
{
    return true;
}
