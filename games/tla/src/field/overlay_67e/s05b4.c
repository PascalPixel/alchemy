#include "ov67e.h"

extern u32 gFrameCount;

static inline s32 GameFlag_Test_(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline, as overlay 679's GameFlag_Test_ does, keeps the actor in r5 and the message in r6 (differ 16, the two swapped, as a direct call). */
    return GameFlag_Test(flag);
}

// The ring of stones: one line after the flag is set, otherwise the full talk and its answer.
void Func_020004bc(s32 actor)
{
    Func_02000d00();
    Func_02000dd0(0);
    if (GameFlag_Test_(0x924)) {
        Func_02000d60(MsgCanYouImagine);
        Func_02000d70(actor, 0);
    } else {
        const u8 *message = MsgSeeThisRing;

        Func_02000d60(message);
        UiText_OpenMessageAtObject(actor, 0);
        if (Inventory_PromptAndSetObjectMode(Func_02000e00(), 0) == 0) {
            Battle_WaitMode0(10);
            Func_02000d60(message + 1);
        } else {
            Battle_WaitMode0(20);
            Func_02000d60(message + 2);
        }
        Func_02000d70(actor, 0);
    }
    Func_02000d08();
}

void Func_02000538(s32 actor)
{
    Func_02000d00();
    Func_02000dd0(0);
    if (GameFlag_Test_(0x924)) {
        Func_02000d60(MsgButItS);
        Func_02000d70(actor, 0);
    } else {
        const u8 *message = MsgSeeThisRingOf;

        Func_02000d60(message);
        UiText_OpenMessageAtObject(actor, 0);
        if (Inventory_PromptAndSetObjectMode(Func_02000e00(), 0) == 0) {
            Battle_WaitMode0(10);
            Func_02000d60(message + 1);
        } else {
            Battle_WaitMode0(20);
            Func_02000d60(message + 2);
        }
        Func_02000d70(actor, 0);
    }
    Func_02000d08();
}

void Func_020005b4(s32 actor)
{
    const u8 *message;

    Func_02000d00();
    Func_02000dd0(0);
    message = MsgLadyUzumeAnd;
    Func_02000d60(message);
    UiText_OpenMessageAtObject(actor, 0);
    if (Inventory_PromptAndSetObjectMode(Func_02000e00(), 0) == 0) {
        Battle_WaitMode0(10);
        Func_02000d60(message + 1);
    } else {
        Battle_WaitMode0(20);
        Func_02000d60(message + 2);
    }
    Func_02000d70(actor, 0);
    Func_02000d08();
}

// Every fourth frame, colours 1 to 14 of palettes 3, 4 and 5 rotate up by one.
void Func_0200060c(void)
{
    volatile u16 *palette;
    s16 saved;
    s32 index;

    if ((gFrameCount & 3) == 0) {
        saved = *(volatile u16 *)0x0500007c;
        index = 14;
    first:
        palette = (u16 *)0x05000060;
        palette[index] = (palette - 1)[index];
        index--;
        if (index != 1)
            goto first;
        *(volatile u16 *)0x05000062 = saved;

        saved = *(volatile u16 *)0x0500009c;
        index = 14;
    second:
        palette = (u16 *)0x05000080;
        palette[index] = (palette - 1)[index];
        index--;
        if (index != 1)
            goto second;
        *(volatile u16 *)0x05000082 = saved;

        saved = *(volatile u16 *)0x050000bc;
        index = 14;
    third:
        palette = (u16 *)0x050000a0;
        palette[index] = (palette - 1)[index];
        index--;
        if (index != 1)
            goto third;
        *(volatile u16 *)0x050000a2 = saved;
    }
}

void Func_020006a4(void)
{
    /* FAKEMATCH: the Call3 and Value calls of call.h set each call's arguments up in the native's order (differ 2 for each one called directly, 12 with all six direct). */
    GameFlag_SetBit(0x929);
    Func_02000d00();
    Func_02000dd0(0);
    Func_02000d60(MsgWait2);
    ObjectMotion_ResetAndSetPositionInMode2(gPartyState.current_owner, 200, 264);
    ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.current_owner);
    Func_02000d70(32, 0);
    Battle_WaitMode0(15);
    Value3((s32 (*)())Func_02000d88, gPartyState.current_owner, 256, 30);
    Call3((void (*)())Func_02000d40, 32, 200 << 16, 216 << 16);
    ObjectMotion_ResetAndSetPositionInMode2(32, 200, 248);
    ObjectMotion_CommitCurrentPositionAndActivate(32);
    Battle_WaitMode0(30);
    Func_02000d70(32, 0);
    ObjectMotion_SetAngleToward(gPartyState.current_owner, 32, 0);
    Battle_WaitMode0(30);
    Object_SetModeById(32, 4);
    Battle_WaitMode0(30);
    Func_02000d70(32, 0);
    Value3((s32 (*)())Func_02000d88, gPartyState.current_owner, 261, 60);
    Func_02000d88(32, 262, 30);
    Func_02000d70(32, 0);
    Object_SetModeById(32, 3);
    Battle_WaitMode0(15);
    Value2((s32 (*)())UiText_OpenMessageAtObject, 32, 0);
    if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 1) {
        Battle_WaitMode0(90);
        Call3((void (*)())ObjectMotion_SetSpeedParameters, 32, 0x6666, 0x3333);
        ObjectMotion_ResetAndSetPositionInMode2(32, 200, 216);
        ObjectMotion_CommitCurrentPositionAndActivate(32);
        Func_02000da8(32, 1);
        Battle_WaitMode0(60);
        Call3((void (*)())ObjectMotion_ArmCallback, 32, 0x4000, 0);
        Func_02000d88(32, 265, 30);
        Func_02000d70(32, 0);
        ObjectMotion_SetActionVariant(32, 3);
        Object_GetById(32)->motion_flags |= 2;
        GameFlag_SetBit(0x302);
    } else {
        Func_02000814();
    }
    Func_02000d08();
}
