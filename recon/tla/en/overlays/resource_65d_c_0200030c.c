/* Near miss, differ 2. What remains: the listing loads gPartyState's address
 * (ldr r5) between the first statement's ands and strb; here sched2 puts it
 * one slot earlier, before the ands. The one-pass block keeps the 0x214
 * offset's movs and lsls after the strb, as the listing has them, but the
 * address load's link to the block's first insn costs 2, which gives it
 * priority 47 over the ands' 46 (sched2 dump).
 * Trials: plain field reads with no block, differ 6 (the address load and the
 * offset's movs scheduled above the ands); Object_GetById_ for either object,
 * no change; Func_02000628_ at the first, second, fourth, sixth and seventh
 * calls as here, differ 6 without the block, and only at the first two,
 * differ 8; the block after the first statement with the address taken after
 * it, differ 2 (the load after the strb); the address taken inside the block,
 * before the first store or through a flags pointer, differ 2; 8-minute
 * permute runs of this form and of the plain form, nothing below differ 2. */
#include "../../../../games/tla/src/field/overlay_65d/local.h"
#include "scene_ids.h"

void Func_020001e0(void);

s32 Func_0200030c(void)
{
    struct PartyState *party;

    Object_GetById(16)->priority_flags &= ~1;
    party = &gPartyState;
    /* FAKEMATCH: the loop note keeps the offset of current_owner after the first store; without it the offset and the address load move above the ands (differ 6). */
    do { } while (0);
    Object_GetById(party->current_owner)->priority_flags |= 0x20;
    if (party->scene == (s32)&SceneId_Ov65dRoom2) {
        if (GameFlag_Test(0x200)) {
            Func_02000620(0);
            Func_02000620(1);
            Func_02000620(2);
            Func_02000618(56, 46, 30, 46, 16, 16);
            Func_02000618(56, 174, 30, 174, 16, 16);
            Func_02000628_(28, 23, 16, 16, 15, 23);
            Func_02000628_(28, 90, 2, 2, 21, 88);
            Func_02000628(28, 90, 4, 4, 17, 89);
            Func_02000628_(28, 90, 2, 1, 17, 91);
            Func_02000628(28, 90, 2, 2, 19, 91);
            Func_02000628_(28, 92, 2, 2, 15, 92);
            Func_02000628_(30, 92, 2, 2, 17, 92);
        }
    } else if (GameFlag_Test(0x85b)) {
        Func_020001e0();
    }
    return 0;
}
