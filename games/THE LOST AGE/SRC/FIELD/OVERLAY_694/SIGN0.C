#include "TYPES.H"
#include "text/MSG_IDS.H"

void Func_0200406c(void);
void Func_02004204(s32 a0);
void UiText_ShowPositionedMessageAndWait(s32 a0, s32 a1);
void Func_02004074(void);
void Func_0200412c(s32 a0);
s32 Func_0200414c(s32 a0, s32 a1);

void Func_020008ec(void)
{
    Func_0200406c();
    Func_02004204(0);
    UiText_ShowPositionedMessageAndWait((s32)MsgExaminedTheWallThereAre, 1);
    Func_02004074();
}

void Func_0200090c(void)
{
    Func_0200406c();
    Func_02004204(0);
    UiText_ShowPositionedMessageAndWait((s32)MsgExaminedTheSignTheCharacters, 1);
    Func_02004074();
}

void Func_0200092c(void)
{
    Func_0200406c();
    Func_02004204(0);
    UiText_ShowPositionedMessageAndWait((s32)MsgTurnTheSlotAndGet, 1);
    Func_02004074();
}

void Func_0200094c(void)
{
    Func_0200406c();
    Func_02004204(0);
    UiText_ShowPositionedMessageAndWait((s32)MsgDropSomeCashAndDouble, 1);
    Func_02004074();
}

void Func_0200096c(void)
{
    Func_0200406c();
    Func_02004204(0);
    UiText_ShowPositionedMessageAndWait((s32)MsgYouNeverKnowWhatThe, 1);
    Func_02004074();
}

void Func_0200098c(void)
{
    Func_0200406c();
    Func_02004204(0);
    UiText_ShowPositionedMessageAndWait((s32)MsgShamanEntranceLocked, 1);
    Func_02004074();
}

void Func_020009ac(void)
{
    Func_0200406c();
    Func_02004204(0);
    Func_0200412c((s32)MsgDoYouKnowAnythingAbout);
    Func_0200414c(8, 0);
    Func_02004074();
}
