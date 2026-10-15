#include "types.h"
#include "call.h"

s32  GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Battle_WaitMode0(s32 frames);
void UiText_OpenMessageAtObject(s32 id, s32 flags);
s32  Inventory_PromptAndSetObjectMode(s32 id, s32 flags);
void Func_02001c74(s32 message);
void Func_02001c8c(s32 a, s32 b);
s32  Func_02001d34(void);

extern u8 MsgAlhafra03[];
extern u8 MsgAlhafra04[];
extern u8 MsgAlhafra16[];
extern u8 MsgAlhafra33[];

void Func_0200013c(s32 actor)
{
    s32 msg;

    if (Value1((s32 (*)())GameFlag_Test, 0x8b0) == 0)
    {
        msg = (s32)&MsgAlhafra03;
        Func_02001c74(msg);
        UiText_OpenMessageAtObject(actor, 0);
        if (Inventory_PromptAndSetObjectMode(Func_02001d34(), 0) == 0)
        {
            Func_02001c74(msg + 1);
            Call1((void (*)())GameFlag_SetBit, 0x8b0);
        }
        else
        {
            Func_02001c74(msg + 2);
        }
        Func_02001c8c(actor, 0);
    }
    else
    {
        Func_02001c74((s32)&MsgAlhafra04);
        Func_02001c8c(actor, 0);
    }
}

void Func_020001a4(s32 actor)
{
    s32 msg = (s32)&MsgAlhafra16;

    Func_02001c74(msg);
    UiText_OpenMessageAtObject(actor, 0);
    if (Inventory_PromptAndSetObjectMode(Func_02001d34(), 0) == 0)
    {
        Battle_WaitMode0(10);
        Func_02001c74(msg + 1);
    }
    else
    {
        Battle_WaitMode0(20);
        Func_02001c74(msg + 2);
    }
    Func_02001c8c(actor, 0);
}

void Func_020001f0(s32 actor)
{
    s32 msg = (s32)&MsgAlhafra33;

    Func_02001c74(msg);
    UiText_OpenMessageAtObject(actor, 0);
    if (Inventory_PromptAndSetObjectMode(Func_02001d34(), 0) == 0)
    {
        Battle_WaitMode0(10);
        Func_02001c74(msg + 1);
    }
    else
    {
        Battle_WaitMode0(20);
        Func_02001c74(msg + 2);
    }
    Func_02001c8c(actor, 0);
}
