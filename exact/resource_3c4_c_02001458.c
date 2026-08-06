/*
 * resource_3c4 @ 0x02001458 (84 bytes: 80 code and one pool word).
 *
 * This is the selector-reversed sibling immediately before 0x020014ac and is
 * written in that owner's proven shape.  It initializes query 0x200, tests
 * flag 0x201, then mirrors the queried state into slot 14's byte at +98 and
 * bit 3 of the byte at +89.  The zero halfword at 0x02001456 is alignment
 * after the preceding owner, not part of this one.
 *
 * Every call site bls its own veneer, so the callees are named at the decoded
 * per-site addresses.
 */
#include "types.h"

void Func_0200453c(s32);
s32 Func_0200453a(s32);
u8 *Func_0200457c(s32);
u8 *Func_02004588(s32);
u8 *Func_02004598(s32);
u8 *Func_020045a4(s32);

void Func_02001458(void)
{
    u8 *flags;
    u8 value;

    Func_0200453c(0x200);
    if (Func_0200453a(0x201) != 0) {
        Func_0200457c(14)[98] = 0;
        Func_02004588(14)[89] &= (u8)0xf7;
    } else {
        Func_02004598(14)[98] = 1;
        flags = Func_020045a4(14);
        flags += 89;
        value = 8;
        value |= *flags;
        *flags = value;
    }
}
