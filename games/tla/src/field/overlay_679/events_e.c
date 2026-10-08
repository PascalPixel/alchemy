#include "local.h"

void Func_02000de0(s32 actor)
{
    s32 angle = Object_GetById(gPartyState.current_owner)->angle;

    /* FAKEMATCH: the native loads the angle before building the quarter turn. */
    __asm__ volatile("" : "+r"(angle));
    if ((u16)((angle + 0x2000) & 0xc000) == 0xc000) {
        Func_02001e24(8, actor);
        return;
    }
    Func_02001cc4();
    Func_02001dcc(0);
    Func_02001d44((s32)&MsgWowYouLook);
    Func_02001d54(actor, 0);
    Func_02001ccc();
}

void Func_02000e40(s32 actor)
{
    s32 msg;

    Func_02001cc4();
    Func_02001dcc(0);
    msg = (s32)&MsgYouGuysAren;
    Func_02001d44(msg);
    UiText_OpenMessageAtObject(actor, 0);
    if (Inventory_PromptAndSetObjectMode(Func_02001e14(), 0) == 0) {
        Battle_WaitMode0(10);
        Func_02001d44(msg + 1);
    } else {
        Battle_WaitMode0(20);
        Func_02001d44(msg + 2);
    }
    Func_02001d54(actor, 0);
    Func_02001ccc();
}

void Func_02000e98(s32 actor)
{
    s32 angle = Object_GetById(gPartyState.current_owner)->angle;

    /* FAKEMATCH: the native loads the angle before building the quarter turn. */
    __asm__ volatile("" : "+r"(angle));
    if ((u16)((angle + 0x2000) & 0xc000) == 0xc000) {
        Func_02001e1c(23, actor);
        return;
    }
    Func_02001cc4();
    Func_02001dcc(0);
    Func_02001d44((s32)&MsgItDBe);
    Func_02001d54(actor, 0);
    Func_02001ccc();
}
