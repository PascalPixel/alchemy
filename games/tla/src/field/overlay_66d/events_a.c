#include "local.h"

void Func_02000088(void)
{
    u8 *work = Ram_HeapSlots->event_work;
    struct PartyState *party;
    u8 *flag;

    /* FAKEMATCH: ⚓️ loads the event work before it builds the flag's offset; without the barrier the offset's first half is scheduled ahead of the load (differ 2). */
    asm volatile("");
    flag = work + 0xd38;
    *flag = 1;
    party = &gPartyState;
    /* FAKEMATCH: ⚓️ loads the party state's address before it builds the entrance's offset; read as a plain field the offset's first half is scheduled ahead of the load (differ 2). */
    asm volatile("" : "+l"(party));
    if (party->entrance != 10)
        *flag = 0;
}

const u8 *Func_020000b8(void)
{
    if (GameFlag_Test(0x8ff))
        return Data_02000cfc;
    return Data_02000b4c;
}

void Func_020000d8(void)
{
    s32 msg;

    if (GameFlag_Test_(0x85d) == 0) {
        msg = (s32)&MsgScene66DNo02;
        Func_02000584(msg);
        UiText_OpenMessageAtObject(14, 0);
        if (Inventory_PromptAndSetObjectMode(Func_020005dc(), 0) == 0) {
            Func_02000584(msg + 1);
            GameFlag_SetBit_(0x85d);
        } else {
            Func_02000584(msg + 2);
        }
        Func_02000594(14, 0);
    } else {
        Func_02000584((s32)&MsgScene66DNo03);
        Func_02000594(14, 0);
    }
}

void Func_02000140(void)
{
    s32 msg;

    if (GameFlag_Test(0x85e) == 0) {
        msg = (s32)&MsgScene66DNo07;
        Func_02000584(msg);
        UiText_OpenMessageAtObject(18, 0);
        if (Inventory_PromptAndSetObjectMode(Func_020005dc(), 0) == 0) {
            Func_02000584(msg + 1);
            GameFlag_SetBit(0x85d);
        } else {
            Func_02000584(msg + 2);
        }
        Func_02000594(18, 0);
    } else {
        Func_02000584((s32)&MsgScene66DNo08);
        Func_02000594(14, 0);
    }
}

void Func_020001a8(void)
{
    s32 msg = (s32)&MsgScene66DNo09;

    Func_02000584(msg);
    UiText_OpenMessageAtObject(19, 0);
    if (Inventory_PromptAndSetObjectMode(Func_020005dc(), 0) == 0) {
        Battle_WaitMode0(10);
        Func_02000584(msg + 1);
    } else {
        Battle_WaitMode0(20);
        Func_02000584(msg + 2);
    }
    Func_02000594(19, 0);
}

void Func_020001f0(s32 actor)
{
    s32 angle = Object_GetById(gPartyState.current_owner)->angle;

    /* FAKEMATCH: the native loads the angle before building the quarter turn. */
    __asm__ volatile("" : "+r"(angle));
    if ((u16)((angle + 0x2000) & 0xc000) == 0x8000) {
        Func_020005e4(16, actor);
        return;
    }
    Func_0200054c();
    Func_020005c4(0);
    Func_02000584((s32)&MsgScene66DNo21);
    Func_02000594(actor, 0);
    Func_02000554();
}

void Func_02000250(s32 actor)
{
    s32 angle = Object_GetById(gPartyState.current_owner)->angle;

    /* FAKEMATCH: the native loads the angle before building the quarter turn. */
    __asm__ volatile("" : "+r"(angle));
    if ((u16)((angle + 0x2000) & 0xc000) == 0x8000) {
        Func_020005e4(16, actor);
        return;
    }
    Func_0200054c();
    Func_020005c4(0);
    Func_02000584((s32)&MsgScene66DNo43);
    Func_02000594(actor, 0);
    Func_02000554();
}

void Func_020002b0(s32 actor)
{
    s32 angle = Object_GetById(gPartyState.current_owner)->angle;

    /* FAKEMATCH: the native loads the angle before building the quarter turn. */
    __asm__ volatile("" : "+r"(angle));
    if ((u16)((angle + 0x2000) & 0xc000) == 0xc000) {
        Func_020005ec(actor);
        return;
    }
    Func_0200054c();
    Func_020005c4(0);
    Func_02000584((s32)&MsgScene66DNo23);
    Func_02000594(26, 0);
    Func_02000554();
}

void Func_0200030c(s32 actor)
{
    s32 angle = Object_GetById(gPartyState.current_owner)->angle;

    /* FAKEMATCH: the native loads the angle before building the quarter turn. */
    __asm__ volatile("" : "+r"(angle));
    if ((u16)((angle + 0x2000) & 0xc000) == 0xc000) {
        Func_020005ec(actor);
        return;
    }
    Func_0200054c();
    Func_020005c4(0);
    Func_02000584((s32)&MsgScene66DNo45);
    Func_02000594(26, 0);
    Func_02000554();
}
