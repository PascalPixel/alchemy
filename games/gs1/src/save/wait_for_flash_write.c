#include "flash.h"

/*
 * The callee stored at 0x02004c1c is invoked through the compiler's r1
 * indirect-call veneer, so it takes one real argument.  The explicit byte
 * narrowing mirrors the three byte-valued inputs at the call boundary.
 */
typedef s32 (*Callee_02004c1c)(s32 argument);

void StartFlashTimer(s32 timing_index);

s32 WaitForFlashWrite(u8 value, s32 argument, u8 expected)
{
    u32 packed = value;
    u32 narrowed = packed;
    s32 local_argument = argument;
    u32 local_expected = expected;
    s32 result;
    Callee_02004c1c *callee_slot;

    narrowed = (narrowed << 24) >> 24;
    local_expected = (local_expected << 24) >> 24;
    result = 0;
    StartFlashTimer(narrowed);
    callee_slot = (Callee_02004c1c *)0x02004c1c;
    packed = (narrowed | 0xc000) << 16;
    goto loop;

failure:
    if (*(u16 *)(*(u8 **)0x02004c08 + 20) == 0x1cc2)
        *(u8 *)0x0e005555 = 0xf0;
    result = packed >> 16;
    goto done;

loop:
    if ((u8)(*callee_slot)(local_argument) == local_expected)
        goto done;
    if (*(u8 *)0x02004c24 == 0)
        goto loop;
    if ((u8)(*callee_slot)(local_argument) != local_expected)
        goto failure;

done:
    StopFlashTimer();
    return result;
}
