/*
 * BYTE-EXACT and adopted 2026-08-07 with no compiler flags: the countdown had
 * been written as `while (frames-- != 0)', which tests the pre-decrement value
 * and puts the `subs' ahead of the call.  The reference tests the counter at
 * the top of the loop and decrements it inside the body, after the call.
 */
#include "types.h"

extern void Func_02001fc0();
void Func_02000324(u8 *o, s32 limit)
{
    s32 frames = 60;

    while (frames != 0) {
        Func_02001fc0(1);
        frames--;
        if (*(s32 *)(o + 12) <= limit) break;
    }
}
