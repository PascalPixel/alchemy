/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target. Resolved with tools/veneer_resolve.ts.
 *
 * 0x0300013c is the relocated ARM ratio helper, established in
 * games/gs1/semantic/main/080e15e8.c as (numerator, denominator) -- TWO arguments.
 * This draft passed three, inventing a trailing zero. Checked against the ROM
 * rather than trusted: at the call, r0 takes arg0, r1 takes 0x10000 and r3
 * the callee, and NOTHING writes r2. Two it is, which independently
 * corroborates 080e15e8's reading at a second call site.
 */
#include "types.h"
#include "global_cells.h"

typedef s32 (*ArmRatio)(s32 numerator, s32 denominator);

struct Work_080936a0 {
    u8 filler0[848];
    u32 previous;
    u32 current;
    u16 kind;
    u16 flags;
};

s32 Runtime_AllocateBlock(s32 arg0, s32 arg1);
void ScheduleCallbackAfterFrames(const void *arg0, s32 arg1);
extern u8 Data_080935d5;

void BattleEffect_ScheduleRatioTransition(s32 arg0, s32 arg1)
{
    struct Work_080936a0 *state = *(struct Work_080936a0 **)ADDR_03001E70;
    s32 handle;
    s32 result;

    handle = Runtime_AllocateBlock(27, 0xccc);
    if (*(s16 *)(handle + 414) != 3)
        return;
    {
        ArmRatio ratio = (ArmRatio)0x0300013c;
        result = ratio(arg0, 0x10000);
    }
    state->previous = state->current;
    state->current = result;
    state->kind = arg1;
    state->flags = 0;
    ScheduleCallbackAfterFrames(&Data_080935d5, 0xc94);
}
