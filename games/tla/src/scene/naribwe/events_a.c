#include "local.h"

void Func_02000088(void)
{
    u8 *work = Ram_HeapSlots->eventWork;
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
        msg = (s32)&MsgNaribwe02;
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
        Func_02000584((s32)&MsgNaribwe03);
        Func_02000594(14, 0);
    }
}

void Func_02000140(void)
{
    s32 msg;

    if (GameFlag_Test(0x85e) == 0) {
        msg = (s32)&MsgNaribwe07;
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
        Func_02000584((s32)&MsgNaribwe08);
        Func_02000594(14, 0);
    }
}

void Func_020001a8(void)
{
    s32 msg = (s32)&MsgNaribwe09;

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
    s32 angle = Object_GetById(gPartyState.currentOwner)->angle;

    /* FAKEMATCH: the native loads the angle before building the quarter turn. */
    __asm__ volatile("" : "+r"(angle));
    if ((u16)((angle + 0x2000) & 0xc000) == 0x8000) {
        Func_020005e4(16, actor);
        return;
    }
    Func_0200054c();
    Func_020005c4(0);
    Func_02000584((s32)&MsgNaribwe21);
    Func_02000594(actor, 0);
    Func_02000554();
}

void Func_02000250(s32 actor)
{
    s32 angle = Object_GetById(gPartyState.currentOwner)->angle;

    /* FAKEMATCH: the native loads the angle before building the quarter turn. */
    __asm__ volatile("" : "+r"(angle));
    if ((u16)((angle + 0x2000) & 0xc000) == 0x8000) {
        Func_020005e4(16, actor);
        return;
    }
    Func_0200054c();
    Func_020005c4(0);
    Func_02000584((s32)&MsgNaribwe43);
    Func_02000594(actor, 0);
    Func_02000554();
}

void Func_020002b0(s32 actor)
{
    s32 angle = Object_GetById(gPartyState.currentOwner)->angle;

    /* FAKEMATCH: the native loads the angle before building the quarter turn. */
    __asm__ volatile("" : "+r"(angle));
    if ((u16)((angle + 0x2000) & 0xc000) == 0xc000) {
        Func_020005ec(actor);
        return;
    }
    Func_0200054c();
    Func_020005c4(0);
    Func_02000584((s32)&MsgNaribwe23);
    Func_02000594(26, 0);
    Func_02000554();
}

void Func_0200030c(s32 actor)
{
    s32 angle = Object_GetById(gPartyState.currentOwner)->angle;

    /* FAKEMATCH: the native loads the angle before building the quarter turn. */
    __asm__ volatile("" : "+r"(angle));
    if ((u16)((angle + 0x2000) & 0xc000) == 0xc000) {
        Func_020005ec(actor);
        return;
    }
    Func_0200054c();
    Func_020005c4(0);
    Func_02000584((s32)&MsgNaribwe45);
    Func_02000594(26, 0);
    Func_02000554();
}

static __inline__ void Func_02000534_(const void *script, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline loads the table's fields as the native does, the script last (differ 12 as a direct call). */
    Func_02000534(script, x, z);
}

void Func_02000368(void)
{
    u8 *work = Ram_HeapSlots->eventWork;
    s32 index;
    u32 id;

    Func_0200054c();
    Func_020005c4(0);
    for (id = 8; id <= 63; id++) {
        u8 *actor = (u8 *)Object_GetById(id);

        if (actor != NULL) {
            u8 *flags = &actor[85];
            /* FAKEMATCH: the zero pinned to r3 is rebuilt on every pass as the native does; loop.c hoists a plain zero out of the loop into a saved register (differ 16). */
            register s32 zero asm("r3") = 0;

            *flags = zero;
        }
    }
    work += 0x154;
    index = *(s16 *)work;
    /* FAKEMATCH: the empty asms on each side of the call keep the entrance's subtraction after it; sched2 ranks the subtraction above the call (differ 2 and 4 without the first and second). */
    asm("");
    Func_020005f4(158);
    asm("");
    index -= 4;
    Func_02000534_(Data_0200064c[index].script, Data_0200064c[index].x, Data_0200064c[index].z);
    ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x8000, 0x4000);
    Object_GetById(gPartyState.currentOwner)->motionFlags = 0;
    Object_SetModeById(gPartyState.currentOwner, 2);
    Func_020005a4(*(s16 *)work);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02000554();
}
