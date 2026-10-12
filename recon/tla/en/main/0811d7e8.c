/*
 * Draft of Func_0811d7e8, a third target list builder. 4 instructions
 * differ: the listing builds the address of the halfword at 74 before it
 * stores the side byte (adds r2, r5; adds r2, #74; strb r0, [r5]), the draft
 * stores first. Tried: the side stored after the two constants, after the
 * field address is taken (same 4), a barrier on the address (13).
 */
#include "types.h"
#include "btlpty.h"
#include "iwram_call.h"

struct ActionTargets {
    u8 side;
    u8 count;
    u8 unknown_02;
    u8 ids[14];
    u8 state_a[14];
    u8 state_b[14];
    u8 state_c[14];
    u8 state_d[14];
    u16 value_4a;
    s32 message;
    s32 value_50;
    s32 value_54;
    s32 cue;
    s32 unknown_5c;
    s32 extra;
    s32 unknown_64;
};

s32 Func_0811a39c(s32 side, u16 *unit_ids);
s32 Func_0811f4d4();
s32 Battle_GetTaggedSlotValue();

void Func_0811d7e8(u16 *source, struct ActionTargets *targets)
{
    u16 ids[10];
    s32 count;
    s32 i;
    s32 side = 2;
    s32 own;

    if (source[0] > 7)
        side = 1;
    count = Func_0811a39c(side, ids);
    Iwram_ClearWords(targets, sizeof *targets);
    own = 0;
    if (source[0] <= 7)
        own = 1;
    targets->side = Battle_GetTaggedSlotValue(Func_0811f4d4(own));
    targets->value_50 = 2;
    targets->value_54 = 14;
    targets->value_4a = source[3];
    for (i = 0; i < count; i++) {
        targets->ids[i] = ids[i];
        targets->state_a[i] = 0;
        targets->state_b[i] = 1;
        targets->state_c[i] = 0;
        targets->state_d[i] = 1;
    }
    targets->count = count;
    targets->message = 403;
    targets->cue = 0x801a;
}
