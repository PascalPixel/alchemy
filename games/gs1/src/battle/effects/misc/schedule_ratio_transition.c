/*
 * Record a ratio-driven transition on the battle effect work block and
 * schedule its callback.
 *
 * The ratio helper is reached through a call-via-register veneer, so it is a
 * typed indirect call rather than a call to a function at the branch target.
 * It takes exactly two arguments, numerator and denominator; r2 is never set
 * at the call site, so a third argument must not be added.
 */
#include "types.h"
#include "scene.h"
#include "abi/battle/effects/misc/schedule_ratio_transition.h"
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
extern u8 gRom;

void BattleFx_ScheduleRatioTransition(s32 arg0, s32 arg1)
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
    ScheduleCallbackAfterFrames(&gRom, 0xc94);
}
