/*
 * Drafts of Func_020004bc and Func_02000538 (overlay 67E, 020084bc and
 * 02008538). differ 16 each, one swap: ⚓️ keeps the actor in r5 and the
 * message in r6, the draft the other way round. The prompt written out in
 * the else branch, with the message declared in the block or at the top,
 * gives the same allocation as this inline.
 */
#include "../../../../games/tla/src/field/overlay_67e/ov67e.h"

static __inline__ void Ask(s32 actor, const u8 *message)
{
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

void Func_020004bc(s32 actor)
{
    Func_02000d00();
    Func_02000dd0(0);
    if (GameFlag_Test(2340)) {
        Func_02000d60(MsgCanYouImagine);
        Func_02000d70(actor, 0);
    } else {
        Ask(actor, MsgSeeThisRing);
    }
    Func_02000d08();
}

void Func_02000538(s32 actor)
{
    Func_02000d00();
    Func_02000dd0(0);
    if (GameFlag_Test(2340)) {
        Func_02000d60(MsgButItS);
        Func_02000d70(actor, 0);
    } else {
        Ask(actor, MsgSeeThisRingOf);
    }
    Func_02000d08();
}
