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

    if (ChatUI == nullptr)
    {
        static TSubclassOf<UChattingUserWidget> ChatWidgetClass = LoadClass<UChattingUserWidget>(nullptr, TEXT("/Game/Blueprints/WBP_ChattingUserWidget.WBP_ChattingUserWidget_C"));
        if (ChatWidgetClass)
        {
            // 'this'를 소유자로 전달해서 위젯을 생성
            ChatUI = CreateWidget<UChattingUserWidget>(this, ChatWidgetClass);
            if (ChatUI)
            {
                ChatUI->AddToViewport();
                UE_LOG(LogTemp, Warning, TEXT("ChatUI successfully created and added to viewport."));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to create ChatUI widget."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("ChatWidgetClass not found."));
        }
    }
    SetupUI();
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

bool AChattingPlayerController::ServerSendGuessMessage_Validate(const int32& PlayerNumber, const FString& GuessNumber)
{
    // 입력된 문자의 길이가 3자리가 아니면 취소
    if (GuessNumber.Len() != 3) return false;

    // 입력된 문자가 숫자가 아닐 시 취소
    TSet<TCHAR> UniqueChars;
    for (int32 i = 0; i < 3; i++)
    {
        if (!FChar::IsDigit(GuessNumber[i])) return false;
        UniqueChars.Add(GuessNumber[i]);
    }
    // 입력된 문자가 중복될 시 취소
    return UniqueChars.Num() == 3;
}

void AChattingPlayerController::ServerSendGuessMessage_Implementation(const int32& PlayerNumber, const FString& GuessNumber)
{
    // 3자리 숫자인지 확인하고 게임 로직 처리
    if (GuessNumber.Len() == 3 && GuessNumber.IsNumeric())
    {
        UE_LOG(LogTemp, Warning, TEXT("AChattingPlayerController : ServerSendGuessMessage : Be Num"));
        // 플레이어 상태 확인
        ABaseBallPlayerState* PS = GetPlayerState<ABaseBallPlayerState>();
        if (PS && PS->TryCount > 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("AChattingPlayerController : ServerSendGuessMessage : Be TryCount"));
            // 시도 횟수 차감
            PS->UseTry();

            // 게임 모드에 추측 전달
            ABaseballGameMode* BaseballGM = Cast<ABaseballGameMode>(GetWorld()->GetAuthGameMode());
            if (BaseballGM)
            {
                UE_LOG(LogTemp, Warning, TEXT("AChattingPlayerController : ServerSendGuessMessage : Send GameMode"));
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
        UE_LOG(LogTemp, Warning, TEXT("ServerSendChatMessage RPC called with: %s"), *FormattedMessage);
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
