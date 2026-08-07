/*
 * BYTE-EXACT and adopted 2026-08-07 with no compiler flags: the only residual
 * was the register pair around the +0x23 OR.  Writing the flag as a local that
 * is initialised first and ORed with the loaded byte afterwards puts the flag
 * in r3 and the byte in r2, which is the reference's order.
 */
#include "types.h"

extern u8 * Func_02001e6e(s32);
extern u8 * Func_02001e7e(s32);
extern u8 * Func_02001e8e(s32);
void Func_02000468(void)
{
    u8 *leader = Func_02001e6e(0);
    if (*(s32 *)(leader + 12) >= 0x100000) {
        u8 *entry = Func_02001e7e(8);
        u32 bit = 2;
        bit = bit | entry[0x23];
        entry[0x23] = (u8)bit;
    } else {
        Func_02001e8e(8)[0x23] = 1;
    }
}
