#include "ChattingPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "ChattingGameState.h"  // GameState 헤더 포함

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
    // 서버 권한을 통해 소유주를 판별한다
    FString PlayerRole = HasAuthority() ? TEXT("Host") : TEXT("Guest");
    FString PlayerName = PlayerRole + TEXT(": "); // "Host: " 또는 "Guest: "

    UE_LOG(LogTemp, Warning, TEXT("AChattingPlayerController::SendChatMessage called with: %s"), *Message);

    ServerSendChatMessage(PlayerName, Message);
}

void AChattingPlayerController::SetupUI()
{
    FInputModeUIOnly InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    SetInputMode(InputMode);
    bShowMouseCursor = true;
}

void AChattingPlayerController::ServerSendChatMessage_Implementation(const FString& PlayerName, const FString& Message)
{
    AChattingGameState* ChatGameState = GetWorld()->GetGameState<AChattingGameState>();
    if (ChatGameState)
    {
        FString FormattedMessage = PlayerName + Message;
        ChatGameState->MulticastReceiveChatMessage(FormattedMessage);
        UE_LOG(LogTemp, Warning, TEXT("ServerSendChatMessage RPC called with: %s"), *FormattedMessage);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ChatGameState is nullptr"));
    }
}

bool AChattingPlayerController::ServerSendChatMessage_Validate(const FString& PlayerName, const FString& Message)
{
    return true;
}
