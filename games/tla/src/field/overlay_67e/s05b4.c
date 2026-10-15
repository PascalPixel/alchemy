#include "ov67e.h"

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
