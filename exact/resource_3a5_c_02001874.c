/*
 * BYTE-EXACT and adopted 2026-08-07 with -fthumb-arg-before-shift-in-sheet.
 * Two residuals closed: the first argument of 0x020035fe is 0x8000
 * (`movs r1, #128' shifted left 8), not 0x10000; and the reference writes
 * `movs r0, #8' ahead of that shift even though the shift is not the last
 * setup insn of the argument sheet.
 */

#include "types.h"

/* Configure actor slot 8's scale, pose, offset, and final pose. */




extern void Func_020035fe();
extern void Func_02003636();
extern void Func_02003620();
extern void Func_02003648();
void Func_02001874(void)
{
    Func_020035fe(8, 0x8000, 0x4000);
    Func_02003636(8, 1);
    Func_02003620(8, 168, 96);
    Func_02003648(8, 2);
}
